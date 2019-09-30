#pragma once
#include <DirectXMath.h>
#include <string>
#include "Vertex.h"
#include "VertexBuffer.h"
#include "Shaders.h"
#include "ConstantBuffers.h"
#include "ConstantBufferTypes.h"

using namespace DirectX;

class XYZIndicator;

class Camera
{
public:
	friend class XYZIndicator;

	Camera();
	~Camera();

	// Get/Set world camera position.
	XMVECTOR GetPositionXM()const;
	XMFLOAT3 GetPosition()const;
	void SetPosition(float x, float y, float z);
	void SetPosition(const XMFLOAT3& v);
	void AdjustPosition(float x, float y, float z);
	void AdjustPosition(const XMFLOAT3& v);
	void AdjustPosition(const XMVECTOR& v);

	// Get camera basis vectors.
	XMVECTOR GetRightXM()const;
	XMFLOAT3 GetRight()const;
	XMVECTOR GetUpXM()const;
	XMFLOAT3 GetUp()const;
	XMVECTOR GetLookXM()const;
	XMFLOAT3 GetLook()const;

	// Get frustum properties.
	float GetNearZ()const;
	float GetFarZ()const;
	float GetAspect()const;
	float GetFovY()const;
	float GetFovX()const;

	// Get near and far plane dimensions in view space coordinates.
	float GetNearWindowWidth()const;
	float GetNearWindowHeight()const;
	float GetFarWindowWidth()const;
	float GetFarWindowHeight()const;

	// Set frustum.
	void SetLens(float fovY, float aspect, float zn, float zf);

	// Define camera space via LookAt parameters.
	void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
	void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);

	// Get View/Proj matrices.
	XMMATRIX View()const;
	XMMATRIX Proj()const;
	XMMATRIX ViewProj()const;

	// Strafe/Walk the camera a distance d.
	void Strafe(float d);
	void Walk(float d);

	// Rotate the camera.
	void Pitch(float angle);
	void Yaw(float angle);
	void Roll(float angle);

	// After modifying camera position/orientation, call to rebuild the view matrix.
	void UpdateViewMatrix();

private:

	// Camera coordinate system with coordinates relative to world space.
	XMFLOAT3 mPosition;
	XMFLOAT3 mRight;
	XMFLOAT3 mUp;
	XMFLOAT3 mLook;

	// Cache frustum properties.
	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;
	float mNearWindowHeight;
	float mFarWindowHeight;

	// Cache View/Proj matrices.
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
};

class XYZIndicator
{
public:
	void Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const std::wstring& vertexShaderPath, const std::wstring& pixelShaderPath);
	void Draw(ID3D11DeviceContext* deviceContext, const Camera& camera);
private:
	VertexShader m_vertexShader;
	PixelShader m_pixelShader;

	VertexBuffer<Vertex> m_vertexBuffer;

	XMMATRIX m_viewMatrix;
	ConstantBuffer<CBufferVertex> cbuffer;
};