#pragma once
#include "stdafx.h"
#include "GeneralShader.h"

class TutorialShader: public GeneralShader
{
public:
	TutorialShader();
	bool Render(ID3D11DeviceContext *pDeviceContext, int indexCount, 
		XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView *texture) override;

private:
	unsigned int SetPolygonLayout() override;
	bool SetShaderParams(ID3D11DeviceContext *pDeviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection);
};

