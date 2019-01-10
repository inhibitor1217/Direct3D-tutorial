#pragma once
#include "stdafx.h"
#include "UIShader.h"

class TextShader : public UIShader
{
public:
	TextShader();

	size_t GetPSUniformVariableSize() override;

	static bool CreatePSUniformVariable(void *&ptr);
	static void SetPSUniformVariable(void *ptr, XMFLOAT4 color, float size);

private:
	struct PSUniformVariableType {
		XMFLOAT4 color;
		float offset;
	};

	bool LoadTextureSampler(ID3D11Device *pDevice) override;
};

