#pragma once
#include "stdafx.h"

class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass &other);
	~D3DClass();

	bool Init(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	void Shutdown();

	void BeginScene(float r, float g, float b, float a);
	void EndScene();

	ID3D11Device *GetDevice();
	ID3D11DeviceContext *GetDeviceContext();
	void GetProjectionMatrix(XMMATRIX &projectionMatrix);
	void GetWorldMatrix(XMMATRIX &worldMatrix);
	void GetOrthoMatrix(XMMATRIX &orthoMatrix);
	void GetVideoCardInfo(char *cardName, int &memory);

private:
	bool	m_vsync_enabled = false;
	int		m_videoCardMemory = 0;
	char	m_videoCardDescription[128] = { 0, };

	IDXGISwapChain			*m_pSwapChain;
	ID3D11Device			*m_pDevice;
	ID3D11DeviceContext		*m_pDeviceContext;
	ID3D11RenderTargetView	*m_pRenderTargetView;
	ID3D11Texture2D			*m_pDepthStencilBuffer;
	ID3D11DepthStencilState *m_pDepthStencilState;
	ID3D11DepthStencilView	*m_pDepthStencilView;
	ID3D11RasterizerState	*m_pRasterState;
	
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;
};

