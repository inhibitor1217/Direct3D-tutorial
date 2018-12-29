#include "Graphics.h"



Graphics::Graphics()
{
	m_pDirect3D = nullptr;
	m_pCamera	= nullptr;
	m_pModel	= nullptr;
	m_pShader	= nullptr;
}


Graphics::Graphics(const Graphics &other)
{
}


Graphics::~Graphics()
{
}


bool Graphics::Init(INT screenWidth, INT screenHeight, HWND hwnd)
{
	m_pDirect3D = (D3D *)malloc(sizeof(D3D));
	if (!m_pDirect3D)
		return false;

	if (!m_pDirect3D->Init(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR)) {
		MessageBox(hwnd, "Could not initialize Direct3D.", "Error", MB_OK);
		return false;
	}

	m_pCamera = new Camera();
	if (!m_pCamera)
		return false;
	m_pCamera->SetPosition(0.0f, 0.0f, -5.0f);

	m_pModel = new TextureModel("./Assets/Textures/grass-top.bmp");
	if (!m_pModel)
		return false;
	if (!m_pModel->Init(m_pDirect3D->GetDevice(), m_pDirect3D->GetDeviceContext())) {
		MessageBox(hwnd, "Could not initialize the model object", "Error", MB_OK);
		return false;
	}

	m_pShader = new TextureShader();
	if (!m_pShader)
		return false;
	if (!m_pShader->Init(m_pDirect3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, "Could not initialize the shader object", "Error", MB_OK);
		return false;
	}

	return true;
}


void Graphics::Shutdown()
{
	if (m_pShader)
		m_pShader->Shutdown();

	if (m_pModel)
		m_pModel->Shutdown();

	if (m_pDirect3D)
		m_pDirect3D->Shutdown();

	Memory::SafeDelete(m_pShader);
	Memory::SafeDelete(m_pModel);
	Memory::SafeDelete(m_pCamera);
	Memory::SafeDelete(m_pDirect3D);
}


bool Graphics::Frame()
{
	return Render();
}


bool Graphics::Render()
{
	m_pDirect3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_pCamera->Render();

	XMMATRIX world, view, projection;
	m_pDirect3D->GetWorldMatrix(world);
	m_pCamera->GetViewMatrix(view);
	m_pDirect3D->GetProjectionMatrix(projection);

	m_pModel->Render(m_pDirect3D->GetDeviceContext());

	if (!m_pShader->Render(m_pDirect3D->GetDeviceContext(), m_pModel->GetIndexCount(), world, view, projection, m_pModel->GetTexture()))
		return false;

	m_pDirect3D->EndScene();

	return true;
}