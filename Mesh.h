#pragma once
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffers.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Texture.h"
#include <vector>

class Mesh
{
public:
	Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<Vertex_Pos_Normal_TexC>& vertices,
		std::vector<UINT>& indices, std::vector<Texture>& textures, const DirectX::XMMATRIX& transformMatrix);
	Mesh(const Mesh& mesh);
	void Draw();
	const DirectX::XMMATRIX& GetTransformMatrix();
private:
	VertexBuffer<Vertex_Pos_Normal_TexC> vertexBuffer;
	IndexBuffer indexBuffer;
	ID3D11DeviceContext* deviceContext;
	std::vector<Texture> textures;
	DirectX::XMMATRIX transformMatrix;
};