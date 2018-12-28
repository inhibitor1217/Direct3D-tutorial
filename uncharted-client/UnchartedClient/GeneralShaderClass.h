#pragma once
#include "stdafx.h"

class GeneralShaderClass: public AlignedAllocationPolicy<16>
{
public:
	GeneralShaderClass();
	GeneralShaderClass(const GeneralShaderClass &other);
	~GeneralShaderClass();

	virtual bool Init(ID3D11Device *pDevice, HWND hwnd);
	virtual void Shutdown();
	virtual bool Render(ID3D11DeviceContext *pDeviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix) = 0;

protected:
	struct MatrixBufferType {
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	ID3D11VertexShader			*m_pVertexShader;
	ID3D11PixelShader			*m_pPixelShader;
	ID3D11InputLayout			*m_pLayout;
	ID3D11Buffer				*m_pMatrixBuffer;
	D3D11_INPUT_ELEMENT_DESC	*m_pPolygonLayout;

	LPCSTR m_vsFilename;
	LPCSTR m_psFilename;
	LPCSTR m_vsEntryPoint;
	LPCSTR m_psEntryPoint;

	virtual bool InitShader(ID3D11Device *pDevice, HWND hwnd);
	virtual unsigned int SetPolygonLayout() = 0;
	virtual void ShutdownShader();
	virtual void OutputShaderErrorMsg(ID3D10Blob *errorMsg, HWND hwnd, LPCSTR shaderFilename);
	virtual void RenderShader(ID3D11DeviceContext *pDeviceContext, int indexCount);
};

