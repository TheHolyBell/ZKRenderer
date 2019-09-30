#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <string>
#include "Vertex.h"
#include "Shaders.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <WICTextureLoader.h>
#include <DirectXMath.h>

using Microsoft::WRL::ComPtr;

template<typename T>
class SimpleObject
{
	VertexShader m_vertexShader;
	PixelShader m_pixelShader;

	VertexBuffer<T> m_vertexBuffer;
	IndexBuffer m_indexBuffer;

	ComPtr<ID3D11Resource> m_texture = nullptr;
	ComPtr<ID3D11ShaderResourceView> m_textureView = nullptr;

	DirectX::XMMATRIX modelMatrix;
public:
	void InitializeBuffers(ID3D11Device* device, T* vertexData, UINT vertexDataSize, UINT* indexData, UINT indexDataSize, XMMATRIX worldMatrix = XMMatrixIdentity())
	{
		modelMatrix = worldMatrix;
		m_vertexBuffer.Initialize(device, vertexData, vertexDataSize);
		m_indexBuffer.Initialize(device, indexData, indexDataSize);
	}
	void InitializeShaders(ID3D11Device* device, const std::wstring& vertexShaderPath, D3D11_INPUT_ELEMENT_DESC* layoutDesc, UINT layoutDescSize, const std::wstring& pixelShaderPath, bool realTimeCompiling = false, std::wstring texturePath = L"nothing")
	{
		m_vertexShader.Initialize(device, vertexShaderPath, layoutDesc, layoutDescSize, realTimeCompiling);
		m_pixelShader.Initialize(device, pixelShaderPath, realTimeCompiling);
		if (texturePath != L"nothing")
			CreateWICTextureFromFile(device, texturePath.c_str(), &m_texture, &m_textureView);
	}

	void SetPosition(float x, float y, float z)
	{
		modelMatrix = XMMatrixTranslation(x, y, z);
	}

	void Draw(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX viewProjectionMatrix, ConstantBuffer<CBufferVertex>& m_constantBuffer)
	{
		UINT offset = 0;

		deviceContext->IASetInputLayout(m_vertexShader.GetInputLayout());
		deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOfBuffer(), m_vertexBuffer.GetStridePtr(), &offset);
		deviceContext->IASetIndexBuffer(m_indexBuffer.GetBuffer(), DXGI_FORMAT_R32_UINT, 0);
		deviceContext->VSSetShader(m_vertexShader.GetVertexShader(), nullptr, 0);
		deviceContext->PSSetShader(m_pixelShader.GetPixelShader(), nullptr, 0);
		deviceContext->PSSetShaderResources(0, 1, m_textureView.GetAddressOf());

		m_constantBuffer.data.wvp = XMMatrixTranspose(modelMatrix * viewProjectionMatrix);
		m_constantBuffer.ApplyChanges();

		deviceContext->VSSetConstantBuffers(0, 1, m_constantBuffer.GetBufferAddr());

		deviceContext->DrawIndexed(m_indexBuffer.GetNumElements(), 0, 0);
	}
};