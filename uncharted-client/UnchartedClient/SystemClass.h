#pragma once
#include "stdafx.h"
#include "InputClass.h"
#include "GraphicsClass.h"

class SystemClass
{
public:
	SystemClass(HINSTANCE hInstance);
	SystemClass(const SystemClass &other);
	~SystemClass();

	bool Init();
	void Shutdown();
	int Run();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	LPCSTR			m_applicationName;
	HINSTANCE		m_hInstance;
	HWND			m_hwnd;
	InputClass		*m_pInput = nullptr;
	GraphicsClass	*m_pGraphics = nullptr;

	bool Frame();
	bool InitWindows(int &screenWidth, int &screenHeight);
	void ShutdownWindows();
};

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
static SystemClass *g_pApplicationHandle = 0;

