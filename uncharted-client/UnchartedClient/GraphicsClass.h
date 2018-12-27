#pragma once
#include "stdafx.h"
#include "D3DClass.h"

const bool FULL_SCREEN   = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR  = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass &other);
	~GraphicsClass();

	bool Init(INT screenWidth, INT screenHeight, HWND hwnd);
	void Shutdown();
	bool Frame();

private:
	D3DClass *m_pDirect3D;

	bool Render();
};

