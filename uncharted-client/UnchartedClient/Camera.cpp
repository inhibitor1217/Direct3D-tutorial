#include "Camera.h"



Camera::Camera()
{
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}


Camera::Camera(const Camera &other)
{
}


Camera::~Camera()
{
}


void Camera::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}


void Camera::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}


XMFLOAT3 Camera::GetPosition()
{
	return m_position;
}


XMFLOAT3 Camera::GetRotation()
{
	return m_rotation;
}


void Camera::GetViewMatrix(XMMATRIX &viewMatrix)
{
	viewMatrix = m_viewMatrix;
}


void Camera::Render()
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