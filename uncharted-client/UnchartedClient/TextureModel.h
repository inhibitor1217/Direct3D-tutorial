#pragma once
#include "RawModel.h"
#include "Texture.h"

class TextureModel: public RawModel
{
public:
	TextureModel();
	TextureModel(Mesh *mesh, Texture *texture);

	ID3D11ShaderResourceView *GetTexture();

private:
	Texture	*m_pTexture;
};

