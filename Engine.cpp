#include "Engine.h"

void Engine::Initialize(HINSTANCE hInstance, int Width, int Height, std::wstring vertexShaderPath, std::wstring pixelShaderPath, bool realTimeCompiling)
{
	timer.Start();

	InitAndCreateWindow(hInstance, Width, Height, L"DirectX", WS_OVERLAPPEDWINDOW);
	m_gfx.Initialize(GetWindowHandler(), Width, Height, vertexShaderPath, pixelShaderPath, realTimeCompiling);
}

void Engine::Render()
{
	m_gfx.RenderFrame();
}

void Engine::Update()
{
	float dt = timer.GetMilisecondsElapsed();
	timer.Restart();

	float cameraSpeed = 0.005f;

	if (GetAsyncKeyState(VK_CONTROL))
		cameraSpeed = 0.05f;

	if (GetAsyncKeyState('W'))
		m_gfx.m_camera.AdjustPosition(m_gfx.m_camera.GetLookXM() * cameraSpeed * dt);
	if (GetAsyncKeyState('S'))
		m_gfx.m_camera.AdjustPosition(-m_gfx.m_camera.GetLookXM() * cameraSpeed * dt);
	if (GetAsyncKeyState('A'))
		m_gfx.m_camera.AdjustPosition(-m_gfx.m_camera.GetRightXM() * cameraSpeed * dt);
	if (GetAsyncKeyState('D'))
		m_gfx.m_camera.AdjustPosition(m_gfx.m_camera.GetRightXM() * cameraSpeed * dt);
	if (GetAsyncKeyState(VK_SPACE))
		m_gfx.m_camera.AdjustPosition(0.0f, cameraSpeed * dt, 0.0f);
	if (GetAsyncKeyState(VK_SHIFT))
		m_gfx.m_camera.AdjustPosition(0.0f, -cameraSpeed * dt, 0.0f);

	m_gfx.m_camera.UpdateViewMatrix();
}

bool Engine::ProcessMessages()
{
	MSG msg = {};


	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Check if the window has closed
	if (msg.message == WM_NULL)
	{
		if (!IsWindow(m_hwnd))
		{
			m_hwnd = nullptr;
			UnregisterClass(m_windowClassName.c_str(), m_hInstance);
			return false;
		}

	}

	return true;
}
