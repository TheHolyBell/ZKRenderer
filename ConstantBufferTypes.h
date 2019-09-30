#pragma once
#include <DirectXMath.h>
#include "Light.h"

struct CBufferVertex
{
	DirectX::XMMATRIX wvp;
};

struct CB_VS_vertexshader
{
	DirectX::XMMATRIX wvpMatrix;
	DirectX::XMMATRIX worldMatrix;
};

struct CB_PS_pixelshader
{
	Light light;
};