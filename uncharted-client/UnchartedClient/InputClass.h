#pragma once
#include "stdafx.h"

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass &other);
	~InputClass();

	void Init();

	void KeyDown(UINT key);
	void KeyUp(UINT key);
	bool IsKeyDown(UINT key);

private:
	bool m_keys[256];
};

