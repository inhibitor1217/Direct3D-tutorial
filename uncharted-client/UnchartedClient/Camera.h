#pragma once
#include "stdafx.h"

class Camera: public AlignedAllocationPolicy<16>
{
public:
	Camera();
	Camera(const Camera &other);
	~Camera();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX &viewMatrix);

private:
	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMMATRIX m_viewMatrix;
};

