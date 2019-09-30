#include "Graphics.h"

void Graphics::Initialize(HWND hwnd, int Width, int Height, std::wstring vertexShaderPath, std::wstring pixelShaderPath, bool runtimeBuild)
{
	fpsTimer.Start();

	InitializeDirectX11(hwnd, Width, Height);
	InitializeShaders(vertexShaderPath, pixelShaderPath, runtimeBuild);
	InitializeBuffers();
	InitializeScene();

	//Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(m_device.Get(), m_deviceContext.Get());
	ImGui::StyleColorsLight();
}

void Graphics::OnResize(int Width, int Height)
{
	m_Width = Width;
	m_Height = Height;

	m_renderTargetView.Reset();
	m_depthStencilView.Reset();
	m_depthBuffer.Reset();

	if (m_swapChain == nullptr)
		return;

	m_swapChain->ResizeBuffers(1, m_Width, m_Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	ComPtr<ID3D11Texture2D> swapChainBufferReference = nullptr;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &swapChainBufferReference);
	m_device->CreateRenderTargetView(swapChainBufferReference.Get(), nullptr, &m_renderTargetView);

	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = m_Width;
	depthStencilDesc.Height = m_Height;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.SampleDesc.Count = 8;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

	m_device->CreateTexture2D(&depthStencilDesc, nullptr, &m_depthBuffer);
	m_device->CreateDepthStencilView(m_depthBuffer.Get(), nullptr, &m_depthStencilView);

	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	D3D11_VIEWPORT viewport = {};

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Height = m_Height;
	viewport.Width = m_Width;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// set the viewport
	m_deviceContext->RSSetViewports(1, &viewport);

	float aspectRatio = (float)Width / (float)Height;

	m_camera.SetLens(XM_PIDIV2, aspectRatio, 0.1f, 1000.0f);
	m_camera.UpdateViewMatrix();
}

void Graphics::RenderFrame()
{
	static bool vsync = true, drawCameraAxis = false;
	static int r = 0, g = 0, b = 0, a = 255;
	float bgcolor[] = { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), bgcolor);
	m_deviceContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_deviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	m_deviceContext->RSSetState(nullptr);
	m_deviceContext->OMSetDepthStencilState(nullptr, 0);


	cube.Draw(m_deviceContext.Get(), m_camera.ViewProj(), m_constantBuffer);
	grass.Draw(m_deviceContext.Get(), m_camera.ViewProj(), m_constantBuffer);

	{
		m_deviceContext->IASetInputLayout(vertexShader.GetInputLayout());
		m_deviceContext->VSSetShader(vertexShader.GetVertexShader(), nullptr, 0);
		m_deviceContext->PSSetShader(pixelShader.GetPixelShader(), nullptr, 0);
		
		cb_ps_pixelshader.data.light = m_light;
		cb_ps_pixelshader.ApplyChanges();
		m_deviceContext->PSSetConstantBuffers(0, 1, cb_ps_pixelshader.GetBufferAddr());

		gameObject.Draw(m_camera.ViewProj());
	}
	//Draw Text
	static int fpsCounter = 0;
	static std::string fpsString = "FPS: 0";
	fpsCounter += 1;
	if (fpsTimer.GetMilisecondsElapsed() > 1000.0)
	{
		fpsString = "FPS: " + std::to_string(fpsCounter);
		fpsCounter = 0;
		fpsTimer.Restart();
	}
	spriteBatch->Begin();
	spriteFont->DrawString(spriteBatch.get(), StringHelper::StringToWide(fpsString).c_str(), DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
	spriteBatch->End();

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Background Color Control Panel");
	ImGui::SliderInt("Red Component: ", &r, 0, 255);
	ImGui::SliderInt("Green Component: ", &g, 0, 255);
	ImGui::SliderInt("Blue Component: ", &b, 0, 255);
	ImGui::SliderInt("Alpha Component: ", &a, 0, 255);
	ImGui::End();

	ImGui::Begin("User Control Panel");

	if (ImGui::RadioButton("VSYNC  ON", vsync))
		vsync = true;

	ImGui::SameLine();

	if (ImGui::RadioButton("Camera Axis ON", drawCameraAxis))
		drawCameraAxis = true;

	if (ImGui::RadioButton("VSYNC OFF", !vsync))
		vsync = false;

	ImGui::SameLine();

	if (ImGui::RadioButton("Camera Axis OFF", !drawCameraAxis))
		drawCameraAxis = false;

	{
		static float pitch = 0.0f, yaw = 0.0f, roll = 0.0f;
		ImGui::DragFloat("Pitch deg: ", &pitch, 0.5f, -180.0f, 180.0f);
		ImGui::DragFloat("Yaw deg: ", &yaw, 0.5f, -180.0f, 180.0f);
		ImGui::DragFloat("Roll deg: ", &roll, 0.5f, -180.0f, 180.0f);

		m_camera.Pitch((float)pitch / XM_PI);
		m_camera.Yaw((float)yaw / XM_PI);
		m_camera.Roll((float)roll / XM_PI);

		pitch = yaw = roll = 0.0f;

		static float x = 0.0f, y = 0.0f, z = -2.0f;
		ImGui::NewLine();
		ImGui::Text("Camera position");
		ImGui::DragFloat("X: ", &x, 0.1f, -50.0f, 50.0f);
		ImGui::DragFloat("Y: ", &y, 0.1f, -50.0f, 50.0f);
		ImGui::DragFloat("Z: ", &z, 0.1f, -50.0f, 50.0f);

		m_camera.AdjustPosition(x, y, z);
		m_camera.UpdateViewMatrix();

		x = y = z = 0.0f;
	}

	ImGui::End();

	{
		static float x = 0.0f, y = 0.0f, z = 0.0f;
		ImGui::Begin("Box Control Panel");
		ImGui::Text("Box Coordinates: ");

		ImGui::SliderFloat("X: ", &x, -50.0f, 50.0f);
		ImGui::SliderFloat("Y: ", &y, -50.0f, 50.0f);
		ImGui::SliderFloat("Z: ", &z, -50.0f, 50.0f);

		cube.SetPosition(x, y, z);
		ImGui::End();
	}


	{
		static float x = 0.0f, y = 0.0f, z = 0.0f;
		ImGui::Begin("Grass Control Panel");
		ImGui::Text("Grass Coordinates: ");
		ImGui::SliderFloat("X: ", &x, -50.0f, 50.0f);
		ImGui::SliderFloat("Y: ", &y, -50.0f, 50.0f);
		ImGui::SliderFloat("Z: ", &z, -50.0f, 50.0f);

		grass.SetPosition(x, y, z);
		ImGui::End();
	}

	{
		static float x = 0.0f, y = 0.0f, z = 0.0f;
		static float rotX = 0.0f, rotY = 0.0f, rotZ = 0.0f;
		ImGui::Begin("Object Control Panel");


		ImGui::Text("Object Coordinates: ");
		ImGui::SliderFloat("X: ", &x, -50.0f, 50.0f);
		ImGui::SliderFloat("Y: ", &y, -50.0f, 50.0f);
		ImGui::SliderFloat("Z: ", &z, -50.0f, 50.0f);
		gameObject.SetPosition(x, y, z);

		ImGui::Text("Object rotation properties: ");

		ImGui::SliderFloat("RotX: ", &rotX, -180.0f, 180.0f);
		ImGui::SliderFloat("RotY: ", &rotY, -180.0f, 180.0f);
		ImGui::SliderFloat("RotZ: ", &rotZ, -180.0f, 180.0f);

		gameObject.SetRotation(rotX / XM_PI, rotY / XM_PI, rotZ / XM_PI);

		ImGui::End();
	}

	{
		ImGui::Begin("Light Position Properties");
		static float x = 0.0f, y = 0.0f, z = 0.0f;
		ImGui::SliderFloat("LightX: ", &x, -50.0f, 50.0f);
		ImGui::SliderFloat("LightY: ", &y, -50.0f, 50.0f);
		ImGui::SliderFloat("LightZ: ", &z, -50.0f, 50.0f);
		m_light.SetPosition(x, y, z);
		ImGui::End();
	}

	//Assemble Together Draw Data
	ImGui::Render();
	//Render Draw Data
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if(drawCameraAxis)
		m_xyzIndicator.Draw(m_deviceContext.Get(), m_camera);

	m_swapChain->Present(vsync, 0);
}

void Graphics::InitializeDirectX11(HWND hwnd, int Width, int Height)
{
	m_Width = Width;
	m_Height = Height;

	DXGI_SWAP_CHAIN_DESC scd = {};

	scd.BufferDesc.Width = Width;
	scd.BufferDesc.Height = Height;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	scd.SampleDesc.Count = 8;
	scd.SampleDesc.Quality = 0;

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hwnd;
	scd.Windowed = true;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr,
		D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
		nullptr, 0, D3D11_SDK_VERSION, &scd,
		&m_swapChain, &m_device, nullptr, &m_deviceContext);

	if (m_swapChain == nullptr)
	{
		MessageBoxA(nullptr, "SwapChain is null. Make sure that you filled everything right", "Error", MB_ICONERROR);
		return;
	}

	ComPtr<ID3D11Texture2D> swapChainBufferReference = nullptr;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &swapChainBufferReference);
	m_device->CreateRenderTargetView(swapChainBufferReference.Get(), nullptr, &m_renderTargetView);

	D3D11_TEXTURE2D_DESC depthStencilDesc = {};

	depthStencilDesc.Width = Width;
	depthStencilDesc.Height = Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 8;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	hr = m_device->CreateTexture2D(&depthStencilDesc, nullptr, &m_depthBuffer);
	m_device->CreateDepthStencilView(m_depthBuffer.Get(), nullptr, &m_depthStencilView);

	m_deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	D3D11_VIEWPORT vp = {};
	vp.Width = Width;
	vp.Height = Height;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	m_deviceContext->RSSetViewports(1, &vp);

	// Initialize Font
	spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_deviceContext.Get());
	spriteFont = std::make_unique<DirectX::SpriteFont>(m_device.Get(), L"Fonts/comic_sans_ms_16.spritefont");
}

void Graphics::InitializeShaders(std::wstring vertexShaderPath, std::wstring pixelShaderPath, bool runtimeBuild)
{
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT numElements = ARRAYSIZE(layoutDesc);

	m_xyzIndicator.Initialize(m_device.Get(), m_deviceContext.Get(), vertexShaderPath, pixelShaderPath);

	cube.InitializeShaders(m_device.Get(), vertexShaderPath, layoutDesc, numElements, pixelShaderPath, runtimeBuild);

	D3D11_INPUT_ELEMENT_DESC textureLayoutDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT texNumElements = ARRAYSIZE(textureLayoutDesc);

	grass.InitializeShaders(m_device.Get(), L"x64/"
#ifdef _DEBUG
		"Debug"
#else
		"Release"
#endif
		"/vertexshader_textures.cso", textureLayoutDesc, 
		texNumElements, L"x64/"
#ifdef _DEBUG
		"Debug"
#else
		"Release"
#endif
		"/pixelshader_textures.cso", false, L"Images/grass.jpg");

	D3D11_INPUT_ELEMENT_DESC posNormalTexLayoutDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT posNormalTexNumElements = ARRAYSIZE(posNormalTexLayoutDesc);

	vertexShader.Initialize(m_device.Get(), L"x64/"
#ifdef _DEBUG
		"Debug"
#else
		"Release"
#endif
		"/vertexshader_lighting.cso", posNormalTexLayoutDesc, posNormalTexNumElements);

	pixelShader.Initialize(m_device.Get(), L"x64/"
#ifdef _DEBUG
		"Debug"
#else
		"Release"
#endif
		"/pixelshader_lighting.cso");
}

void Graphics::InitializeBuffers()
{
	Vertex vertices[] =
	{
		Vertex(-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
		Vertex(-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
		Vertex( 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f),
		Vertex( 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f),

		Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 1.0f),
		Vertex(-0.5f,  0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f),
		Vertex(0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f),
		Vertex(0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f)
	};


	UINT numElementsVertices = ARRAYSIZE(vertices);

	UINT indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	UINT numElementsIndices = ARRAYSIZE(indices);

	cube.InitializeBuffers(m_device.Get(), vertices, numElementsVertices, indices, numElementsIndices);

	m_constantBuffer.Initialize(m_device.Get(), m_deviceContext.Get());

	Vertex_Tex texVertices[] =
	{
		Vertex_Tex(-200.0f, 0.0f, -200.0f, 0.0f, 1.0f),
		Vertex_Tex(-200.0f, 0.0f,  200.0f, 0.0f, 0.0f),
		Vertex_Tex( 200.0f, 0.0f,  200.0f, 1.0f, 0.0f),
		Vertex_Tex( 200.0f, 0.0f, -200.0f, 1.0f, 1.0f),
	};

	UINT texIndices[] =
	{
		0,1,2,
		0,2,3
	};

	grass.InitializeBuffers(m_device.Get(), texVertices, ARRAYSIZE(texVertices), texIndices, ARRAYSIZE(texIndices));
	
	float aspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);
	m_camera.SetLens(XM_PIDIV2, aspectRatio, 0.1f, 1000.0f);
	m_camera.SetPosition(0.0f, 2.0f, -3.0f);
	m_camera.UpdateViewMatrix();
}

void Graphics::InitializeScene()
{
	
	//Initialize Constant Buffer(s)
	cb_vs_vertexshader.Initialize(m_device.Get(), m_deviceContext.Get());
	cb_ps_pixelshader.Initialize(m_device.Get(), m_deviceContext.Get());

	m_camera.SetPosition(0.0f, 2.0f, -2.0f);
	m_camera.SetLens(XM_PIDIV2, static_cast<float>(m_Width) / static_cast<float>(m_Height), 0.1f, 1000.0f);

	// Initialize models
	if (!gameObject.Initialize("Models/nanosuit/nanosuit.obj", m_device.Get(), m_deviceContext.Get(), cb_vs_vertexshader))
		MessageBoxW(nullptr, L"œ»«ƒ¿ ¬—≈ Õ¿ –€ÀŒ—‹ “ŒÀ» ", L"Error", MB_ICONERROR);

	m_light.SetPosition(0.0f, 0.0f, 0.0f);
	m_light.SetDiffuse(1.0f, 1.0f, 1.0f);
	m_light.SetAmbient(1.0f, 1.0f, 1.0f);
}
