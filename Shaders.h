#pragma once
#include <d3d11.h>
#include <string>
#include <wrl/client.h>
#include <d3dcompiler.h>

using Microsoft::WRL::ComPtr;

class VertexShader
{
	ComPtr<ID3D11VertexShader> m_shader = nullptr;
	ComPtr<ID3D10Blob> m_shaderBlob = nullptr;
	ComPtr<ID3D11InputLayout> m_inputLayout = nullptr;

public:
	void Initialize(ID3D11Device* device, std::wstring shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, int numElements, bool runTimeCompile = false);
	ID3D11VertexShader* GetVertexShader() const noexcept;
	ID3D10Blob* GetVertexBlob() const noexcept;
	ID3D11InputLayout* GetInputLayout() const noexcept;
};


class PixelShader
{
	ComPtr<ID3D11PixelShader> m_shader = nullptr;
	ComPtr<ID3D10Blob> m_shaderBlob = nullptr;

public:
	void Initialize(ID3D11Device* device, std::wstring shaderPath, bool runTimeCompile = false);
	ID3D11PixelShader* GetPixelShader() const noexcept;
	ID3D10Blob* GetPixelBlob() const noexcept;
};