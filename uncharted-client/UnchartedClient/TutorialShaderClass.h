#pragma once
#include "stdafx.h"

class TutorialShaderClass: public AlignedAllocationPolicy<16>
{
public:
	TutorialShaderClass();
	TutorialShaderClass(const TutorialShaderClass &other);
	~TutorialShaderClass();

	bool Init(ID3D11Device *pDevice, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext *pDeviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

private:
	struct MatrixBufferType {
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	ID3D11VertexShader	*m_pVertexShader;
	ID3D11PixelShader	*m_pPixelShader;
	ID3D11InputLayout	*m_pLayout;
	ID3D11Buffer		*m_pMatrixBuffer;

	bool InitShader(ID3D11Device *pDevice, HWND hwnd, LPCSTR vsFilename, LPCSTR psFilename);
	void ShutdownShader();
	void OutputShaderErrorMsg(ID3D10Blob *errorMsg, HWND hwnd, LPCSTR shaderFilename);
	bool SetShaderParams(ID3D11DeviceContext *pDeviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection);
	void RenderShader(ID3D11DeviceContext *pDeviceContext, int indexCount);
};

