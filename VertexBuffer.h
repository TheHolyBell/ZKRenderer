#pragma once
#include <d3d11.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

template<typename T>
class VertexBuffer
{
	ComPtr<ID3D11Buffer> buffer = nullptr;
	UINT stride = sizeof(T);

public:
	void Initialize(ID3D11Device* device, T* data, UINT size)
	{
		D3D11_BUFFER_DESC vinitDesc = {};
		vinitDesc.Usage = D3D11_USAGE_DEFAULT;
		vinitDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vinitDesc.ByteWidth = sizeof(T) * size;

		D3D11_SUBRESOURCE_DATA vinitData = {};
		vinitData.pSysMem = data;

		device->CreateBuffer(&vinitDesc, &vinitData, &buffer);
	}

	ID3D11Buffer* GetBuffer() noexcept
	{
		return buffer.Get();
	}

	ID3D11Buffer** GetAddressOfBuffer() noexcept
	{
		return buffer.GetAddressOf();
	}

	UINT GetStride() const noexcept
	{
		return stride;
	}

	const UINT* GetStridePtr() const noexcept
	{
		return &stride;
	}
};

