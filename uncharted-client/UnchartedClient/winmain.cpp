#include "DXApp.h"


class TestApp : public DXApp
{
public:
	TestApp(HINSTANCE hInstance);
	~TestApp();

	bool Init() override;
	void Update(float deltaTime) override;
	void Render(float deltaTime) override;
};


TestApp::TestApp(HINSTANCE hInstance) : DXApp(hInstance)
{

}


TestApp::~TestApp()
{

}


bool TestApp::Init()
{
	return DXApp::Init();
}


void TestApp::Update(float deltaTime)
{

}


void TestApp::Render(float deltaTime)
{
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::CornflowerBlue);
	m_pSwapChain->Present(0, 0);
}


int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstace, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	TestApp tApp(hInstance);

	if (!tApp.Init())
		return 1;

	return tApp.Run();
}