#include "TextureModel.h"



TextureModel::TextureModel(char *filename)
{
	m_pTexture = nullptr;

	strncpy_s(m_textureFilename, filename, sizeof(m_textureFilename));
}


bool TextureModel::Init(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext)
{
	if (!RawModel::Init(pDevice, pDeviceContext))
		return false;

	return LoadTexture(pDevice, pDeviceContext);
}


void TextureModel::Shutdown()
{
	ReleaseTexture();

	RawModel::Shutdown();
}


ID3D11ShaderResourceView *TextureModel::GetTexture()
{
	return m_pTexture->GetTexture();
}


bool TextureModel::LoadTexture(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext)
{
	m_pTexture = new Texture();
	if (!m_pTexture)
		return false;

	return m_pTexture->Init(pDevice, pDeviceContext, m_textureFilename);
}


void TextureModel::ReleaseTexture()
{
	if (m_pTexture)
		m_pTexture->Shutdown();

	Memory::SafeDelete(m_pTexture);
}