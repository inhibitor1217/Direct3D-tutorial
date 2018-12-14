#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include "DXUtil.h"

class DXApp
{
public:
	DXApp(HINSTANCE hInstance);
	virtual ~DXApp();

	/* Main Application Loop */
	int Run();

	/* Framework Methods */
	virtual bool Init();
	virtual void Update(float deltaTime) = 0;
	virtual void Render(float deltaTime) = 0;
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void EnableAlphaBlending(bool enable);
	void EnableZBuffer(bool enable);

	ID3D11Device *GetDevice();
	ID3D11DeviceContext *GetContext();

protected:
	/* Win32 Attributes */
	HWND			m_hAppWnd;
	HINSTANCE		m_hAppInstance;
	UINT			m_ClientWidth;
	UINT			m_ClientHeight;
	std::string		m_AppTitle;
	DWORD			m_WndStyle;

	/* DirectX Attributes */
	ID3D11Device			*m_pDevice;
	ID3D11DeviceContext		*m_pImmediateContext;
	IDXGISwapChain			*m_pSwapChain;
	ID3D11RenderTargetView	*m_pRenderTargetView;
	ID3D11Texture2D			*m_pDepthStencilBuffer;
	ID3D11DepthStencilState *m_pDepthStencilState;
	ID3D11DepthStencilState *m_pDepthDisabledStencilState;
	ID3D11DepthStencilView  *m_pDepthStencilView;
	ID3D11RasterizerState	*m_pRasterState;
	ID3D11BlendState		*m_pAlphaEnableBlendingState;
	ID3D11BlendState		*m_pAlphaDisableBlendingState;
	D3D_DRIVER_TYPE			m_DriverType;
	D3D_FEATURE_LEVEL		m_FeatureLevel;
	D3D11_VIEWPORT			m_Viewport;

	bool InitWindow();
	bool InitDirect3D();
};

