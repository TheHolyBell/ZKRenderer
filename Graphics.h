#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>
#include <Windows.h>
#include <string>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "Shaders.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Vertex.h"
#include "ConstantBufferTypes.h"
#include "ConstantBuffers.h"
#include "Camera.h"
#include <WICTextureLoader.h>
#include "SimpleObject.h"
#include "GameObject.h"
#include "Timer.h"
#include <memory>
#include "Light.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

using Microsoft::WRL::ComPtr;

class Graphics
{
public:
	void Initialize(HWND hwnd, int Width, int Height, std::wstring vertexShaderPath, std::wstring pixelShaderPath, bool runtimeBuild);
	void OnResize(int Width, int Height);
	void RenderFrame();
	Camera m_camera;
	XYZIndicator m_xyzIndicator;
	Light m_light;
private:
	void InitializeDirectX11(HWND hwnd, int Width, int Height);
	void InitializeShaders(std::wstring vertexShaderPath, std::wstring pixelShaderPath, bool runtimeBuild);
	void InitializeBuffers();
	void InitializeScene();

	ComPtr<ID3D11Device> m_device = nullptr;
	ComPtr<ID3D11DeviceContext> m_deviceContext = nullptr;
	ComPtr<ID3D11RenderTargetView> m_renderTargetView = nullptr;
	ComPtr<ID3D11Texture2D> m_depthBuffer = nullptr;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView = nullptr;

	ComPtr<IDXGISwapChain> m_swapChain = nullptr;
	
	SimpleObject<Vertex> cube;
	SimpleObject<Vertex_Tex> grass;

	ConstantBuffer<CBufferVertex> m_constantBuffer;

	ConstantBuffer<CB_VS_vertexshader> cb_vs_vertexshader;
	ConstantBuffer<CB_PS_pixelshader> cb_ps_pixelshader;

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	GameObject gameObject;

	VertexShader vertexShader;
	PixelShader pixelShader;

	int m_Width;
	int m_Height;

	Timer fpsTimer;
};