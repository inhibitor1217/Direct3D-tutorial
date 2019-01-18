#pragma once
#include "stdafx.h"
#include "D3D.h"
#include "Camera.h"
#include "Font.h"
#include "TextureModel.h"
#include "UIText.h"
#include "TextShader.h"
#include "Input.h"
#include <vector>

const bool FULL_SCREEN   = true;
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
	bool Frame(Input *pInput);

private:
	D3D				*m_pDirect3D = nullptr;
	Camera			*m_pCamera = nullptr;
	
	RawModel		*m_pModel = nullptr;
	UIModel			*m_pUIModel = nullptr;
	UIText			*m_pUIText = nullptr;

	GeneralShader	*m_pTextureShader = nullptr;
	GeneralShader	*m_pUIShader = nullptr;
	GeneralShader	*m_pTextShader = nullptr;

	void			*m_pTextureVSShaderVariables = nullptr;
	void			*m_pTextVSShaderVariables = nullptr;
	void			*m_pTextPSShaderVariables = nullptr;

	std::vector<Mesh *> m_meshes;
	std::vector<Font *> m_fonts;
	std::vector<Texture *> m_textures;

	bool Render();
};

