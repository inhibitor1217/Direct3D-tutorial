#pragma once
#include "stdafx.h"
#include "UIShader.h"

class TextShader : public UIShader
{
public:
	TextShader();

	size_t GetPSUniformVariableSize() override;

	static bool CreatePSUniformVariable(void *&ptr);
	static void SetPSUniformVariable(void *ptr, XMFLOAT4 color);

private:
	struct PSUniformVariableType {
		XMFLOAT4 color;
	};

	bool LoadTextureSampler(ID3D11Device *pDevice) override;
};

