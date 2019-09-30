#include "Shaders.h"

// VERTEX SHADER DEFINITION

void VertexShader::Initialize(ID3D11Device* device, const std::wstring& shaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, int numElements, bool runTimeCompile)
{
	if (runTimeCompile)
	{
		ComPtr<ID3D10Blob> errorBuffer = nullptr;
		D3DCompileFromFile(shaderPath.c_str(), nullptr, nullptr, "main", "vs_5_0", 0, 0, &m_shaderBlob, &errorBuffer);

		if (errorBuffer != nullptr)
		{
			MessageBoxA(nullptr, (char*)errorBuffer->GetBufferPointer(), "You screw up", MB_ICONERROR);
			return;
		}
	}
	else
		D3DReadFileToBlob(shaderPath.c_str(), &m_shaderBlob);

	device->CreateVertexShader(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), nullptr, &m_shader);
	device->CreateInputLayout(layoutDesc, numElements, m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), &m_inputLayout);
}

ID3D11VertexShader* VertexShader::GetVertexShader() const noexcept
{
	return m_shader.Get();
}

ID3D10Blob* VertexShader::GetVertexBlob() const noexcept
{
	return m_shaderBlob.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout() const noexcept
{
	return m_inputLayout.Get();
}

// PIXEL SHADER DEFINITION

void PixelShader::Initialize(ID3D11Device* device, const std::wstring& shaderPath, bool runTimeCompile)
{
	if (runTimeCompile)
	{
		ComPtr<ID3D10Blob> errorBuffer = nullptr;
		D3DCompileFromFile(shaderPath.c_str(), nullptr, nullptr, "main", "ps_5_0", 0, 0, &m_shaderBlob, &errorBuffer);

		if (errorBuffer != nullptr)
		{	
			MessageBoxA(nullptr, (char*)errorBuffer->GetBufferPointer(), "You screw up", MB_ICONERROR);
			return;
		}
	}
	else
		D3DReadFileToBlob(shaderPath.c_str(), &m_shaderBlob);

	device->CreatePixelShader(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), nullptr, &m_shader);
}

ID3D11PixelShader* PixelShader::GetPixelShader() const noexcept
{
	return m_shader.Get();
}

ID3D10Blob* PixelShader::GetPixelBlob() const noexcept
{
	return m_shaderBlob.Get();
}
