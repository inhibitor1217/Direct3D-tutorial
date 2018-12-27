#include "D3DClass.h"



D3DClass::D3DClass()
{
	m_pSwapChain			= nullptr;
	m_pDevice				= nullptr;
	m_pDeviceContext		= nullptr;
	m_pRenderTargetView		= nullptr;
	m_pDepthStencilBuffer	= nullptr;
	m_pDepthStencilState	= nullptr;
	m_pDepthStencilView		= nullptr;
	m_pRasterState			= nullptr;
}


D3DClass::D3DClass(const D3DClass &other)
{
}


D3DClass::~D3DClass()
{
}


bool D3DClass::Init(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	m_vsync_enabled = vsync;

	IDXGIFactory *factory = nullptr;
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&factory)))
		return false;

	IDXGIAdapter *adapter = nullptr;
	if (FAILED(factory->EnumAdapters(0, &adapter)))
		return false;

	IDXGIOutput *adapterOutput = nullptr;
	if (FAILED(adapter->EnumOutputs(0, &adapterOutput)))
		return false;

	unsigned int numModes = 0;
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL)))
		return false;

	DXGI_MODE_DESC *displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList)
		return false;

	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList)))
		return false;

	unsigned int numerator = 0;
	unsigned int denominator = 0;
	for (unsigned int i = 0; i < numModes; i++) {
		if (displayModeList[i].Width == static_cast<unsigned int>(screenWidth)) {
			if (displayModeList[i].Height == static_cast<unsigned int>(screenHeight)) {
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	DXGI_ADAPTER_DESC adapterDesc;
	if (FAILED(adapter->GetDesc(&adapterDesc)))
		return false;

	m_videoCardMemory = static_cast<int>(adapterDesc.DedicatedVideoMemory >> 20);

	size_t videoCardDescriptionLength = 0;
	if (wcstombs_s(&videoCardDescriptionLength, m_videoCardDescription, 128, adapterDesc.Description, 128) != 0)
		return false;

	Memory::SafeDeleteArray(displayModeList);
	Memory::SafeRelease(adapterOutput);
	Memory::SafeRelease(adapter);
	Memory::SafeRelease(factory);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = !fullscreen;

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);
	
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, numFeatureLevels,
		D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext)))
		return false;

	ID3D11Texture2D *backBufferPtr = nullptr;
	if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&backBufferPtr))))
		return false;

	if (FAILED(m_pDevice->CreateRenderTargetView(backBufferPtr, NULL, &m_pRenderTargetView)))
		return false;

	Memory::SafeRelease(backBufferPtr);

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.MiscFlags = 0;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	if (FAILED(m_pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer)))
		return false;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.StencilEnable = TRUE;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	if (FAILED(m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState)))
		return false;

	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView)))
		return false;

	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterDesc.AntialiasedLineEnable = FALSE;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = TRUE;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = FALSE;
	rasterDesc.MultisampleEnable = FALSE;
	rasterDesc.ScissorEnable = FALSE;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	if (FAILED(m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState)))
		return false;

	m_pDeviceContext->RSSetState(m_pRasterState);

	D3D11_VIEWPORT viewPort;
	viewPort.Width = static_cast<float>(screenWidth);
	viewPort.Height = static_cast<float>(screenHeight);
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;

	m_pDeviceContext->RSSetViewports(1, &viewPort);

	float FOV = 3.141592654f / 4.0f;
	float screenAspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

	m_projectionMatrix = XMMatrixPerspectiveFovLH(FOV, screenAspectRatio, screenNear, screenDepth);
	m_worldMatrix = XMMatrixIdentity();
	m_orthoMatrix = XMMatrixOrthographicLH(static_cast<float>(screenWidth), static_cast<float>(screenHeight), screenNear, screenDepth);

	return true;
}


void D3DClass::Shutdown()
{
	if (m_pSwapChain)
		m_pSwapChain->SetFullscreenState(false, NULL);

	Memory::SafeRelease(m_pRasterState);
	Memory::SafeRelease(m_pDepthStencilView);
	Memory::SafeRelease(m_pDepthStencilState);
	Memory::SafeRelease(m_pDepthStencilBuffer);
	Memory::SafeRelease(m_pRenderTargetView);
	Memory::SafeRelease(m_pDeviceContext);
	Memory::SafeRelease(m_pDevice);
	Memory::SafeRelease(m_pSwapChain);
}


void D3DClass::BeginScene(float r, float g, float b, float a)
{
	float color[] = { r, g, b, a };

	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}


void D3DClass::EndScene()
{
	if (m_vsync_enabled)
		m_pSwapChain->Present(1, 0);
	else
		m_pSwapChain->Present(0, 0);
}


ID3D11Device *D3DClass::GetDevice()
{
	return m_pDevice;
}


ID3D11DeviceContext *D3DClass::GetDeviceContext()
{
	return m_pDeviceContext;
}


void D3DClass::GetProjectionMatrix(XMMATRIX &projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
}


void D3DClass::GetWorldMatrix(XMMATRIX &worldMatrix)
{
	worldMatrix = m_worldMatrix;
}


void D3DClass::GetOrthoMatrix(XMMATRIX &orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
}


void D3DClass::GetVideoCardInfo(char *cardName, int &memory)
{
	strcpy_s(cardName, 128, m_videoCardDescription);
	memory = m_videoCardMemory;
}