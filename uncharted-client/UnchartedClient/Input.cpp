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


void Input::Init()
{
	for (int i = 0; i < 256; i++) {
		m_keys[i] = false;
	}
}


void Input::KeyDown(UINT key)
{
	m_keys[key] = true;
}


void Input::KeyUp(UINT key)
{
	m_keys[key] = false;
}


bool Input::IsKeyDown(UINT key)
{
	return m_keys[key];
}
