#pragma once
#include "RawModel.h"
#include "Texture.h"

class TextureModel: public RawModel
{
public:
	TextureModel();

	ID3D11ShaderResourceView *GetTexture();
	void SetTexture(Texture *texture);

private:
	Texture	*m_pTexture;
};

