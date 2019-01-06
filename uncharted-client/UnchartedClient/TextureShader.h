#pragma once
#include "stdafx.h"
#include "GeneralShader.h"

class TextureShader: public GeneralShader
{
public:
	TextureShader();

	bool Render(ID3D11DeviceContext *pDeviceContext, int indexCount,
		XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView *texture) override;

protected:
	ID3D11SamplerState *m_pSampleState;

	bool InitShader(ID3D11Device *pDevice, HWND hwnd) override;
	void ShutdownShader() override;
	virtual unsigned int SetPolygonLayout() override;
	virtual bool LoadTextureSampler(ID3D11Device *pDevice);
	bool SetShaderParams(ID3D11DeviceContext *pDeviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView *texture);
	void RenderShader(ID3D11DeviceContext *pDeviceContext, int indexCount) override;
};

