#include "GraphicsClass.h"



GraphicsClass::GraphicsClass()
{
	m_pDirect3D = nullptr;
	m_pCamera	= nullptr;
	m_pModel	= nullptr;
	m_pShader	= nullptr;
}


GraphicsClass::GraphicsClass(const GraphicsClass &other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Init(INT screenWidth, INT screenHeight, HWND hwnd)
{
	m_pDirect3D = (D3DClass *)malloc(sizeof(D3DClass));
	if (!m_pDirect3D)
		return false;

	if (!m_pDirect3D->Init(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR)) {
		MessageBox(hwnd, "Could not initialize Direct3D.", "Error", MB_OK);
		return false;
	}

	m_pCamera = new CameraClass();
	if (!m_pCamera)
		return false;
	m_pCamera->SetPosition(0.0f, 0.0f, -5.0f);

	m_pModel = new RawModelClass();
	if (!m_pModel)
		return false;
	if (!m_pModel->Init(m_pDirect3D->GetDevice())) {
		MessageBox(hwnd, "Could not initialize the model object", "Error", MB_OK);
		return false;
	}

	m_pShader = new TutorialShaderClass();
	if (!m_pShader)
		return false;
	if (!m_pShader->Init(m_pDirect3D->GetDevice(), hwnd)) {
		MessageBox(hwnd, "Could not initialize the shader object", "Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
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


bool GraphicsClass::Frame()
{
	return Render();
}


bool GraphicsClass::Render()
{
	m_pDirect3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_pCamera->Render();

	XMMATRIX world, view, projection;
	m_pDirect3D->GetWorldMatrix(world);
	m_pCamera->GetViewMatrix(view);
	m_pDirect3D->GetProjectionMatrix(projection);

	m_pModel->Render(m_pDirect3D->GetDeviceContext());

	if (!m_pShader->Render(m_pDirect3D->GetDeviceContext(), m_pModel->GetIndexCount(), world, view, projection))
		return false;

	m_pDirect3D->EndScene();

	return true;
}