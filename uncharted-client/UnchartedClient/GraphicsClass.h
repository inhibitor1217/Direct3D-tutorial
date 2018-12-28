#pragma once
#include "stdafx.h"
#include "D3DClass.h"
#include "CameraClass.h"
#include "RawModelClass.h"
#include "TutorialShaderClass.h"

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
	D3DClass			*m_pDirect3D;
	CameraClass			*m_pCamera;
	RawModelClass		*m_pModel;
	GeneralShaderClass  *m_pShader;

	bool Render();
};

