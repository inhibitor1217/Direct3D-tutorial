#pragma once
#include "stdafx.h"

class Input
{
public:
	Input();
	Input(const Input &other);
	~Input();

	void Init();

	void KeyDown(UINT key);
	void KeyUp(UINT key);
	bool IsKeyDown(UINT key);

private:
	bool m_keys[256];
};

