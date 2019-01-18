#include "Input.h"



Input::Input()
{
}


Input::Input(const Input &other)
{
}

Input::~Input()
{
}


bool Input::Init(HINSTANCE hInstance, HWND hwnd, int screenWidth, int screenHeight)
{
	for (int i = 0; i < 256; i++) {
		m_keyboardState[i] = false;
	}

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void **>(&m_pDirectInput), NULL)))
		return false;

	if(FAILED(m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL)))
		return false;

	if (FAILED(m_pKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	if (FAILED(m_pKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		return false;

	if (FAILED(m_pKeyboard->Acquire()))
		return false;

	if (FAILED(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL)))
		return false;

	if (FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		return false;

	if (FAILED(m_pMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		return false;

	if (FAILED(m_pMouse->Acquire()))
		return false;

	return true;
}


void Input::Shutdown()
{
	if (m_pMouse) {
		m_pMouse->Unacquire();
		Memory::SafeRelease(m_pMouse);
	}

	if (m_pKeyboard) {
		m_pKeyboard->Unacquire();
		Memory::SafeRelease(m_pKeyboard);
	}

	Memory::SafeRelease(m_pDirectInput);
}


bool Input::Frame()
{
	if (!ReadKeyboard())
		return false;

	if (!ReadMouse())
		return false;

	ProcessInput();

	return true;
}


void Input::GetMouse(int &x, int &y)
{
	x = m_mouseX;
	y = m_mouseY;
}


bool Input::IsKeyDown(UINT key)
{
	return m_keyboardState[key];
}


bool Input::ReadKeyboard()
{
	HRESULT result;
	if (FAILED(result = m_pKeyboard->GetDeviceState(sizeof(m_keyboardState), reinterpret_cast<void *>(&m_keyboardState)))) {
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			m_pKeyboard->Acquire();
		else
			return false;
	}
	return true;
}


bool Input::ReadMouse()
{
	HRESULT result;
	if (FAILED(result = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), reinterpret_cast<void *>(&m_mouseState)))) {
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			m_pMouse->Acquire();
		else
			return false;
	}
	return true;
}


void Input::ProcessInput()
{
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	if (m_mouseX < 0)
		m_mouseX = 0;
	if (m_mouseX > m_screenWidth)
		m_mouseX = m_screenWidth;

	if (m_mouseY < 0)
		m_mouseY = 0;
	if (m_mouseY > m_screenHeight)
		m_mouseY = m_screenHeight;
}