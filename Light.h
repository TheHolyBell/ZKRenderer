#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Light
{
	XMFLOAT3 Pos;
	float pad1 = 0.0f;
	XMFLOAT3 Diffuse = XMFLOAT3{1.0f, 1.0f, 1.0f};
	float pad2 = 0.0f;
	XMFLOAT3 Ambient = XMFLOAT3{ 1.0f, 1.0f, 1.0f };
	float pad3 = 0.0f;
public:
	void SetPosition(float x, float y, float z);
	void SetAmbient(float r, float g, float b);
	void SetDiffuse(float r, float g, float b);
};