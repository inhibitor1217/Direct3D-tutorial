#pragma once
#include "stdafx.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Input
{
public:
	Input();
	Input(const Input &other);
	~Input();

	bool Init(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight);
	void Shutdown();
	bool Frame();

	void GetMouse(int &x, int &y);
	bool IsKeyDown(UINT key);

private:
	IDirectInput8			*m_pDirectInput = nullptr;
	IDirectInputDevice8		*m_pKeyboard = nullptr;
	IDirectInputDevice8		*m_pMouse = nullptr;

	bool m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth = 0;
	int m_screenHeight = 0;
	int m_mouseX = 0;
	int m_mouseY = 0;

	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();
};