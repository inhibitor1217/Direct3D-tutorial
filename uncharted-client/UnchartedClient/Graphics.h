#pragma once
#include "stdafx.h"
#include "D3D.h"
#include "Camera.h"
#include "Font.h"
#include "TextureModel.h"
#include "UIModel.h"
#include "UIShader.h"

const bool FULL_SCREEN   = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR  = 0.1f;

class Graphics
{
public:
	Graphics();
	Graphics(const Graphics &other);
	~Graphics();

	bool Init(INT screenWidth, INT screenHeight, HWND hwnd);
	void Shutdown();
	bool Frame();

private:
	D3D				*m_pDirect3D;
	Camera			*m_pCamera;
	RawModel		*m_pModel;
	UIModel			*m_pUIModel;
	GeneralShader	*m_pTextureShader;
	GeneralShader	*m_pUIShader;

	// temp
	Mesh *tempMesh = nullptr;

	bool Render();
};

