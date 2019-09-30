#include "GameObject.h"

bool GameObject::Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, ConstantBuffer<CB_VS_vertexshader>& cb_vs_vertexshader)
{
	if (!model.Initialize(filePath, device, deviceContext, cb_vs_vertexshader))
		return false;

	SetPosition(0.0f, 0.0f, 0.0f);
	SetRotation(0.0f, 0.0f, 0.0f);
	UpdateWorldMatrix();
	return true;
}


void GameObject::Draw(const XMMATRIX& viewProjectionMatrix)
{
	model.Draw(worldMatrix, viewProjectionMatrix);
}


void GameObject::UpdateWorldMatrix()
{
	worldMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z) * XMMatrixTranslation(pos.x, pos.y, pos.z);
	XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
	vec_forward = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	vec_backward = XMVector3TransformCoord(DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	vec_left = XMVector3TransformCoord(DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	vec_right = XMVector3TransformCoord(DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}

const XMVECTOR& GameObject::GetPositionVector() const
{
	return posVector;
}

const XMFLOAT3& GameObject::GetPositionFloat3() const
{
	return pos;
}

const XMVECTOR& GameObject::GetRotationVector() const
{
	return rotVector;
}

const XMFLOAT3& GameObject::GetRotationFloat3() const
{
	return rot;
}

void GameObject::SetPosition(const XMVECTOR& pos)
{
	XMStoreFloat3(&this->pos, pos);
	posVector = pos;
	UpdateWorldMatrix();
}

void GameObject::SetPosition(const XMFLOAT3& pos)
{
	this->pos = pos;
	posVector = XMLoadFloat3(&this->pos);
	UpdateWorldMatrix();
}

void GameObject::SetPosition(float x, float y, float z)
{
	pos = XMFLOAT3(x, y, z);
	posVector = XMLoadFloat3(&this->pos);
	UpdateWorldMatrix();
}

void GameObject::AdjustPosition(const XMVECTOR& pos)
{
	posVector += pos;
	XMStoreFloat3(&this->pos, posVector);
	UpdateWorldMatrix();
}

void GameObject::AdjustPosition(const XMFLOAT3& pos)
{
	this->pos.x += pos.y;
	this->pos.y += pos.y;
	this->pos.z += pos.z;
	posVector = XMLoadFloat3(&this->pos);
	UpdateWorldMatrix();
}

void GameObject::AdjustPosition(float x, float y, float z)
{
	pos.x += x;
	pos.y += y;
	pos.z += z;
	posVector = XMLoadFloat3(&pos);
	UpdateWorldMatrix();
}

void GameObject::SetRotation(const XMVECTOR& rot)
{
	rotVector = rot;
	XMStoreFloat3(&this->rot, rot);
	UpdateWorldMatrix();
}

void GameObject::SetRotation(const XMFLOAT3& rot)
{
	this->rot = rot;
	rotVector = XMLoadFloat3(&this->rot);
	UpdateWorldMatrix();
}

void GameObject::SetRotation(float x, float y, float z)
{
	rot = XMFLOAT3(x, y, z);
	rotVector = XMLoadFloat3(&this->rot);
	UpdateWorldMatrix();
}

void GameObject::AdjustRotation(const XMVECTOR& rot)
{
	this->rotVector += rot;
	XMStoreFloat3(&this->rot, this->rotVector);
	this->UpdateWorldMatrix();
}

void GameObject::AdjustRotation(const XMFLOAT3& rot)
{
	this->rot.x += rot.x;
	this->rot.y += rot.y;
	this->rot.z += rot.z;
	rotVector = XMLoadFloat3(&this->rot);
	UpdateWorldMatrix();
}

void GameObject::AdjustRotation(float x, float y, float z)
{
	rot.x += x;
	rot.y += y;
	rot.z += z;
	rotVector = XMLoadFloat3(&rot);
	UpdateWorldMatrix();
}

void GameObject::SetLookAtPos(XMFLOAT3 lookAtPos)
{
	//Verify that look at pos is not the same as cam pos. They cannot be the same as that wouldn't make sense and would result in undefined behavior.
	if (lookAtPos.x == pos.x && lookAtPos.y == pos.y && lookAtPos.z == pos.z)
		return;

	lookAtPos.x = pos.x - lookAtPos.x;
	lookAtPos.y = pos.y - lookAtPos.y;
	lookAtPos.z = pos.z - lookAtPos.z;

	float pitch = 0.0f;
	if (lookAtPos.y != 0.0f)
	{
		const float distance = sqrt(lookAtPos.x * lookAtPos.x + lookAtPos.z * lookAtPos.z);
		pitch = atan(lookAtPos.y / distance);
	}

	float yaw = 0.0f;
	if (lookAtPos.x != 0.0f)
		yaw = atan(lookAtPos.x / lookAtPos.z);
	if (lookAtPos.z > 0)
		yaw += XM_PI;

	SetRotation(pitch, yaw, 0.0f);
}

const XMVECTOR& GameObject::GetForwardVector()
{
	return vec_forward;
}

const XMVECTOR& GameObject::GetRightVector()
{
	return vec_right;
}

const XMVECTOR& GameObject::GetBackwardVector()
{
	return vec_backward;
}

const XMVECTOR& GameObject::GetLeftVector()
{
	return vec_left;
}