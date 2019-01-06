#pragma once
#include "stdafx.h"
#include "UIShader.h"

class TextShader : public UIShader
{
public:
	TextShader();

private:
	bool LoadTextureSampler(ID3D11Device *pDevice) override;
};

