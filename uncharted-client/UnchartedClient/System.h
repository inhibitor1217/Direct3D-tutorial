#pragma once
#include "stdafx.h"
#include "Input.h"
#include "Graphics.h"

class System
{
public:
	System(HINSTANCE hInstance);
	System(const System &other);
	~System();

	bool Init();
	void Shutdown();
	int Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	LPCSTR			m_applicationName;
	HINSTANCE		m_hInstance;
	HWND			m_hwnd;
	Input			*m_pInput = nullptr;
	Graphics		*m_pGraphics = nullptr;

	bool Frame();
	bool InitWindows(int &screenWidth, int &screenHeight);
	void ShutdownWindows();
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
static System *g_pApplicationHandle = 0;

