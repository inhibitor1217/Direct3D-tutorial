#pragma once
#include "stdafx.h"
#include "TextureShader.h"

class UIShader : public TextureShader
{
public:
	UIShader();

private:
	unsigned int SetPolygonLayout() override;
};