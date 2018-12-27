#include "CameraClass.h"



CameraClass::CameraClass()
{
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}


CameraClass::CameraClass(const CameraClass &other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}


XMFLOAT3 CameraClass::GetPosition()
{
	return m_position;
}


XMFLOAT3 CameraClass::GetRotation()
{
	return m_rotation;
}


void CameraClass::GetViewMatrix(XMMATRIX &viewMatrix)
{
	viewMatrix = m_viewMatrix;
}


void CameraClass::Render()
{
	XMFLOAT3 up, position, forward;
	XMVECTOR upVector, positionVector, forwardVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;
	upVector = XMLoadFloat3(&up);
	
	position = m_position;
	positionVector = XMLoadFloat3(&position);

	forward.x = 0.0f;
	forward.y = 0.0f;
	forward.z = 1.0f;
	forwardVector = XMLoadFloat3(&forward);

	pitch = Math::Deg2Radians(m_rotation.x);
	yaw = Math::Deg2Radians(m_rotation.y);
	roll = Math::Deg2Radians(m_rotation.z);

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	forwardVector = XMVector3TransformCoord(forwardVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	forwardVector = XMVectorAdd(positionVector, forwardVector);

	m_viewMatrix = XMMatrixLookAtLH(positionVector, forwardVector, upVector);
}