#pragma once
#include <DirectXMath.h>

using DirectX::XMFLOAT2;
using DirectX::XMFLOAT3;
using DirectX::XMFLOAT4;

struct Vertex
{
	Vertex() = default;
	Vertex(float x, float y, float z, float r, float g, float b, float a)
		: pos(x,y,z), color(r,g,b,a){}
	XMFLOAT3 pos = {};
	XMFLOAT4 color = {};
};

struct Vertex_Tex
{
	Vertex_Tex() = default;
	Vertex_Tex(float x, float y, float z, float u, float v)
		: pos(x, y, z), tex(u, v) {}
	XMFLOAT3 pos = {};
	XMFLOAT2 tex = {};
};

struct Vertex_Pos_Normal_TexC
{
	Vertex_Pos_Normal_TexC() = default;
	Vertex_Pos_Normal_TexC(float x, float y, float z, float nx, float ny, float nz, float u, float v)
		: pos(x, y, z), normal(nx, ny, nz),tex(u, v) {}
	XMFLOAT3 pos = {};
	XMFLOAT3 normal = {};
	XMFLOAT2 tex = {};
};