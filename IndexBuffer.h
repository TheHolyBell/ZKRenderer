#pragma once
#include <d3d11.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class IndexBuffer
{
	ComPtr<ID3D11Buffer> buffer = nullptr;
	UINT numElements;

public:
	void Initialize(ID3D11Device* device, UINT* data, UINT size)
	{
		numElements = size;

		D3D11_BUFFER_DESC iinitDesc = {};
		iinitDesc.Usage = D3D11_USAGE_DEFAULT;
		iinitDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		iinitDesc.ByteWidth = sizeof(UINT) * size;

		D3D11_SUBRESOURCE_DATA iinitData = {};
		iinitData.pSysMem = data;

		device->CreateBuffer(&iinitDesc, &iinitData, &buffer);
	}

	ID3D11Buffer* GetBuffer() noexcept
	{
		return buffer.Get();
	}

	UINT GetNumElements()  noexcept
	{
		return numElements;
	}
};