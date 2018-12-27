#include "DXApp.h"

namespace
{
	DXApp *g_pApp = nullptr;
}


LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (g_pApp)
		return g_pApp->MsgProc(hwnd, msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}


DXApp::DXApp(HINSTANCE hInstance)
{
	/* Initialize WIN32 Attributes */
	m_hAppInstance	= hInstance;
	m_hAppWnd		= NULL;
	m_ClientWidth	= 1280;
	m_ClientHeight	= 720;
	m_AppTitle		= "Uncharted";
	m_WndStyle		= WS_OVERLAPPEDWINDOW;

	/* Initialize Direct3D Attributes */
	m_pDevice						= nullptr;
	m_pImmediateContext				= nullptr;
	m_pRenderTargetView				= nullptr;
	m_pSwapChain					= nullptr;
	m_pDepthStencilBuffer			= nullptr;
	m_pDepthStencilState			= nullptr;
	m_pDepthDisabledStencilState	= nullptr;
	m_pDepthStencilView				= nullptr;
	m_pRasterState					= nullptr;
	m_pAlphaEnableBlendingState		= nullptr;
	m_pAlphaDisableBlendingState	= nullptr;

	g_pApp = this;
}


DXApp::~DXApp()
{
	if (m_pImmediateContext)
		m_pImmediateContext->ClearState();

	Memory::SafeRelease(m_pSwapChain);
	Memory::SafeRelease(m_pAlphaEnableBlendingState);
	Memory::SafeRelease(m_pAlphaDisableBlendingState);
	Memory::SafeRelease(m_pDepthStencilState);
	Memory::SafeRelease(m_pDepthDisabledStencilState);
	Memory::SafeRelease(m_pRasterState);
	Memory::SafeRelease(m_pDepthStencilView);
	Memory::SafeRelease(m_pDepthStencilBuffer);
	Memory::SafeRelease(m_pRenderTargetView);
	Memory::SafeRelease(m_pImmediateContext);
	Memory::SafeRelease(m_pDevice);
}


int DXApp::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			Update(0.0f);
			Render(0.0f);
		}
	}

	return static_cast<int>(msg.wParam);
}


bool DXApp::Init()
{
	if (!InitWindow())
		return false;
	if (!InitDirect3D())
		return false;

	return true;
}


LRESULT DXApp::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}


void DXApp::EnableAlphaBlending(bool enable)
{
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (enable) {
		m_pImmediateContext->OMSetBlendState(m_pAlphaEnableBlendingState, blendFactor, 0xFFFFFFFF);
	}
	else {
		m_pImmediateContext->OMSetBlendState(m_pAlphaDisableBlendingState, blendFactor, 0xFFFFFFFF);
	}
}


void DXApp::EnableZBuffer(bool enable)
{
	if (enable) {
		m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilState, 1);
	}
	else {
		m_pImmediateContext->OMSetDepthStencilState(m_pDepthDisabledStencilState, 1);
	}
}


ID3D11Device *DXApp::GetDevice()
{
	return m_pDevice;
}


ID3D11DeviceContext *DXApp::GetContext()
{
	return m_pImmediateContext;
}


bool DXApp::InitWindow()
{
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbClsExtra			= 0;
	wcex.cbSize				= sizeof(WNDCLASSEX);
	wcex.cbWndExtra			= 0;
	wcex.hbrBackground		= (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon				= LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);
	wcex.hInstance			= m_hAppInstance;
	wcex.lpfnWndProc		= MainWndProc;
	wcex.lpszClassName		= "DXAPPWNDCLASS";
	wcex.lpszMenuName		= NULL;
	wcex.style				= CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wcex)) {
		OutputDebugString("\nFailed: RegisterClassEx\n");
		return false;
	}

	RECT r = { 0, 0, static_cast<WORD>(m_ClientWidth), static_cast<WORD>(m_ClientHeight) };
	AdjustWindowRect(&r, m_WndStyle, FALSE);
	UINT width = r.right - r.left;
	UINT height = r.bottom - r.top;
	UINT x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
	UINT y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

	m_hAppWnd = CreateWindow("DXAPPWNDCLASS", m_AppTitle.c_str(), m_WndStyle,
		x, y, width, height, NULL, NULL, m_hAppInstance, NULL);
	if (!m_hAppWnd) {
		OutputDebugString("\nFailed: CreateWindow\n");
		return false;
	}

	ShowWindow(m_hAppWnd, SW_SHOW);

	return true;
}


bool DXApp::InitDirect3D()
{
	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	HRESULT result;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_BLEND_DESC blendStateDesc;

	ID3D11Texture2D *m_pBackBufferTexture = 0;

	/* Create Swap Chain */
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferCount							= 1;
	swapChainDesc.BufferDesc.Width						= m_ClientWidth;
	swapChainDesc.BufferDesc.Height						= m_ClientHeight;
	swapChainDesc.BufferDesc.Format						= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
	swapChainDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.Flags									= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.OutputWindow							= m_hAppWnd;
	swapChainDesc.SampleDesc.Count						= 1;
	swapChainDesc.SampleDesc.Quality					= 0;
	swapChainDesc.SwapEffect							= DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed								= TRUE;

	for (UINT i = 0; i < numDriverTypes; i++) {
		result = D3D11CreateDeviceAndSwapChain(NULL, driverTypes[i], NULL, createDeviceFlags,
			featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pDevice,
			&m_FeatureLevel, &m_pImmediateContext);
		if (SUCCEEDED(result)) {
			m_DriverType = driverTypes[i];
			break;
		}
	}

	if (FAILED(result)) {
		OutputDebugString("\nFailed: D3D11CreateDeviceAndSwapChain\n");
		return false;
	}

	/* Create Render Target View */
	result = m_pSwapChain->GetBuffer(NULL, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&m_pBackBufferTexture));
	
	if (FAILED(result)) {
		OutputDebugString("\nFailed: IDXGISwapChain.GetBuffer\n");
		return false;
	}
	
	result = m_pDevice->CreateRenderTargetView(m_pBackBufferTexture, nullptr, &m_pRenderTargetView);
	
	if (FAILED(result)) {
		OutputDebugString("\nFailed: ID3D11Device.CreateRenderTargetView\n");
		return false;
	}
	
	Memory::SafeRelease(m_pBackBufferTexture);

	/* Create Depth Buffer */
	ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthBufferDesc.Width				= m_ClientWidth;
	depthBufferDesc.Height				= m_ClientHeight;
	depthBufferDesc.ArraySize			= 1;
	depthBufferDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags		= 0;
	depthBufferDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.MipLevels			= 1;
	depthBufferDesc.MiscFlags			= 0;
	depthBufferDesc.SampleDesc.Count	= 1;
	depthBufferDesc.SampleDesc.Quality	= 0;
	depthBufferDesc.Usage				= D3D11_USAGE_DEFAULT;

	result = m_pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer);
	if (FAILED(result)) {
		OutputDebugString("\nFailed: ID3D11Device.CreateTexture2D\n");
		return false;
	}

	/* Create Depth Stencil Buffer with Depth Enabled */
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.DepthEnable					= TRUE;
	depthStencilDesc.DepthFunc						= D3D11_COMPARISON_LESS;
	depthStencilDesc.DepthWriteMask					= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.StencilEnable					= TRUE;
	depthStencilDesc.StencilReadMask				= 0xFF;
	depthStencilDesc.StencilWriteMask				= 0xFF;

	result = m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
	if (FAILED(result)) {
		OutputDebugString("\nFailed: ID3D11Device.CreateDepthStencilState\n");
		return false;
	}

	/* Create Depth Stencil Buffer with Depth Disabled */
	depthStencilDesc.DepthEnable					= FALSE;

	result = m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthDisabledStencilState);
	if (FAILED(result)) {
		OutputDebugString("\nFailed: ID3D11Device.CreateDepthStencilState\n");
		return false;
	}

	/* Bind Depth Stencil Buffer */
	m_pImmediateContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

	/* Create Depth Stencil View */
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView);
	if (FAILED(result)) {
		OutputDebugString("\nFailed: ID3D11Device.CreateDepthStencilView\n");
		return false;
	}

	/* Bind Render Target View with the Depth Stencil View */
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	/* Create Rasterizer */
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterDesc.AntialiasedLineEnable	= FALSE;
	rasterDesc.CullMode					= D3D11_CULL_BACK;
	rasterDesc.DepthBias				= 0;
	rasterDesc.DepthBiasClamp			= 0.0f;
	rasterDesc.DepthClipEnable			= TRUE;
	rasterDesc.FillMode					= D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise	= FALSE;
	rasterDesc.MultisampleEnable		= FALSE;
	rasterDesc.ScissorEnable			= FALSE;
	rasterDesc.SlopeScaledDepthBias		= 0.0f;

	result = m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState);
	if (FAILED(result)) {
		OutputDebugString("\nFailed: ID3D11Device.CreateRasterizerState\n");
		return false;
	}

	/* Bind Rasterizer */
	m_pImmediateContext->RSSetState(m_pRasterState);

	/* Create Viewport */
	m_Viewport.Width	= static_cast<float>(m_ClientWidth);
	m_Viewport.Height	= static_cast<float>(m_ClientHeight);
	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	/* Bind Viewport */
	m_pImmediateContext->RSSetViewports(1, &m_Viewport);

	/* Create Blend State with Alpha Blending Enabled */
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));
	blendStateDesc.RenderTarget[0].BlendEnable				= TRUE;
	blendStateDesc.RenderTarget[0].BlendOp					= D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].BlendOpAlpha				= D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlend					= D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlendAlpha			= D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask	= 0x0F;

	result = m_pDevice->CreateBlendState(&blendStateDesc, &m_pAlphaEnableBlendingState);
	if (FAILED(result)) {
		OutputDebugString("\nFailed: ID3D11Device.CreateBlendState\n");
		return false;
	}

	/* Create Blend State with Alpha Blending Disabled */
	blendStateDesc.RenderTarget[0].BlendEnable				= FALSE;

	result = m_pDevice->CreateBlendState(&blendStateDesc, &m_pAlphaDisableBlendingState);
	if (FAILED(result)) {
		OutputDebugString("\nFailed: ID3D11Device.CreateBlendState\n");
		return false;
	}

	

	return true;
}