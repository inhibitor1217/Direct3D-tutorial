#pragma once
#include "stdafx.h"
#include "UIModel.h"
#include "Font.h"

class UIText: public UIModel
{
public:
	void SetFont(Font *font);
	Font *GetFont();
	ID3D11ShaderResourceView *GetTexture() override;

private:
	Font *m_pFont;
};

