#pragma once
#include "stdafx.h"
#include "GeneralShaderClass.h"

class TutorialShaderClass: public GeneralShaderClass
{
public:
	TutorialShaderClass();
	bool Render(ID3D11DeviceContext *pDeviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix) override;

private:
	unsigned int SetPolygonLayout() override;
	bool SetShaderParams(ID3D11DeviceContext *pDeviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection);
};

