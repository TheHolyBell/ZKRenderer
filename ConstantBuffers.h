#pragma once
#include <d3d11.h>
#include <wrl/client.h>

template<typename T>
class ConstantBuffer
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;
	UINT m_size = sizeof(T);
public:
	T data;

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* dc)
	{
		m_deviceContext = dc;
		D3D11_BUFFER_DESC cbufferDesc = {};
		cbufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbufferDesc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		cbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		device->CreateBuffer(&cbufferDesc, nullptr, &m_buffer);
	}

	void ApplyChanges()
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		m_deviceContext->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		CopyMemory(mappedResource.pData, &data, sizeof(data));
		m_deviceContext->Unmap(m_buffer.Get(), 0);
	}

	ID3D11Buffer** GetBufferAddr() noexcept
	{
		return m_buffer.GetAddressOf();
	}
};