#include "Mesh.h"

Mesh::Mesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::vector<Vertex_Pos_Normal_TexC>& vertices, std::vector<UINT>& indices, std::vector<Texture>& textures, const DirectX::XMMATRIX& transformMatrix)
	:deviceContext(deviceContext), textures(textures), transformMatrix(transformMatrix)
{
	vertexBuffer.Initialize(device, vertices.data(), vertices.size());

	indexBuffer.Initialize(device, indices.data(), indices.size());
}

Mesh::Mesh(const Mesh& mesh)
{
	deviceContext = mesh.deviceContext;
	indexBuffer = mesh.indexBuffer;
	vertexBuffer = mesh.vertexBuffer;
	textures = mesh.textures;
	transformMatrix = mesh.transformMatrix;
}

void Mesh::Draw()
{
	UINT offset = 0;

	for (auto& i : textures)
	{
		if (i.GetType() == aiTextureType_DIFFUSE)
		{
			deviceContext->PSSetShaderResources(0, 1, i.GetTextureResourceViewAddress());
			break;
		}
	}

	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOfBuffer(), vertexBuffer.GetStridePtr(), &offset);
	deviceContext->IASetIndexBuffer(indexBuffer.GetBuffer(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->DrawIndexed(indexBuffer.GetNumElements(), 0, 0);
}

const DirectX::XMMATRIX& Mesh::GetTransformMatrix()
{
	return transformMatrix;
}
