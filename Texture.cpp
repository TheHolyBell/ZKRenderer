#include "Texture.h"
#include "StringHelper.h"

Texture::Texture(ID3D11Device* device, const Color& color, aiTextureType type)
{
	Initialize1x1ColorTexture(device, color, type);
}

Texture::Texture(ID3D11Device* device, const Color* colorData, UINT width, UINT height, aiTextureType type)
{
	InitializeColorTexture(device, colorData, width, height, type);
}

Texture::Texture(ID3D11Device* device, const std::string& filePath, aiTextureType type)
	: type(type)
{
	if (StringHelper::GetFileExtension(filePath) == ".dds")
	{
		HRESULT hr = DirectX::CreateDDSTextureFromFile(device, StringHelper::StringToWide(filePath).c_str(), &texture, &textureView);
		if (FAILED(hr))
			Initialize1x1ColorTexture(device, Colors::UnloadedTextureColor, type);
		return;
	}
	else
	{
		HRESULT hr = DirectX::CreateWICTextureFromFile(device, StringHelper::StringToWide(filePath).c_str(), &texture, &textureView);
		if (FAILED(hr))
			Initialize1x1ColorTexture(device, Colors::UnloadedTextureColor, type);
		return;
	}
}

Texture::Texture(ID3D11Device* device, const uint8_t* pData, size_t size, aiTextureType type)
	: type(type)
{
	HRESULT hr = DirectX::CreateWICTextureFromMemory(device, pData, size, &texture, &textureView);
	if (FAILED(hr))
		MessageBoxA(nullptr, "Failed to create texture from memory.", "You screw up", MB_ICONERROR);
}

aiTextureType Texture::GetType()
{
	return type;
}

ID3D11ShaderResourceView* Texture::GetTextureResourceView()
{
	return textureView.Get();
}

ID3D11ShaderResourceView** Texture::GetTextureResourceViewAddress()
{
	return textureView.GetAddressOf();
}

void Texture::Initialize1x1ColorTexture(ID3D11Device* device, const Color& colorData, aiTextureType type)
{
	InitializeColorTexture(device, &colorData, 1, 1, type);
}

void Texture::InitializeColorTexture(ID3D11Device* device, const Color* colorData, UINT width, UINT height, aiTextureType type)
{
	this->type = type;
	CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);
	ID3D11Texture2D* p2DTexture = nullptr;
	D3D11_SUBRESOURCE_DATA initialData{};
	initialData.pSysMem = colorData;
	initialData.SysMemPitch = width * sizeof(Color);
	HRESULT hr = device->CreateTexture2D(&textureDesc, &initialData, &p2DTexture);
	
	if (FAILED(hr))
		MessageBoxA(nullptr, "Failed to create texture from memory.", "You screw up", MB_ICONERROR);

	texture = static_cast<ID3D11Texture2D*>(p2DTexture);
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
	hr = device->CreateShaderResourceView(texture.Get(), &srvDesc, textureView.GetAddressOf());

	if (FAILED(hr))
		MessageBoxA(nullptr, "Failed to create shader resource view from texture generated from color data.", "You screw up", MB_ICONERROR);
}