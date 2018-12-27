#include "InputClass.h"



InputClass::InputClass()
{
}


InputClass::InputClass(const InputClass &other)
{
}

InputClass::~InputClass()
{
}


void InputClass::Init()
{
	for (int i = 0; i < 256; i++) {
		m_keys[i] = false;
	}
}


void InputClass::KeyDown(UINT key)
{
	m_keys[key] = true;
}


void InputClass::KeyUp(UINT key)
{
	m_keys[key] = false;
}


bool InputClass::IsKeyDown(UINT key)
{
	return m_keys[key];
}
