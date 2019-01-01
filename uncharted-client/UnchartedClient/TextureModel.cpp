#include "TextureModel.h"



TextureModel::TextureModel()
{
	m_pTexture = nullptr;
}


TextureModel::TextureModel(Mesh *mesh, Texture *texture) : RawModel(mesh)
{
	m_pTexture = texture;
}


ID3D11ShaderResourceView *TextureModel::GetTexture()
{
	return m_pTexture->GetTexture();
}