#include "GraphicsClass.h"



GraphicsClass::GraphicsClass()
{
	m_pDirect3D = nullptr;
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

	return true;
}


void GraphicsClass::Shutdown()
{
	if (m_pDirect3D)
		m_pDirect3D->Shutdown();

	Memory::SafeDelete(m_pDirect3D);
}


bool GraphicsClass::Frame()
{
	return Render();
}


bool GraphicsClass::Render()
{
	m_pDirect3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	m_pDirect3D->EndScene();

	return true;
}