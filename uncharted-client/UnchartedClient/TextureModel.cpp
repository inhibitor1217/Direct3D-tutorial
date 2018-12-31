#include "TextureModel.h"



TextureModel::TextureModel()
{
	m_pTexture = nullptr;
}


ID3D11ShaderResourceView *TextureModel::GetTexture()
{
	return m_pTexture->GetTexture();
}


void TextureModel::SetTexture(Texture *texture)
{
	m_pTexture = texture;
}