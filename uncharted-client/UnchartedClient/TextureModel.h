#pragma once
#include "RawModel.h"
#include "Texture.h"

class TextureModel: public RawModel
{
public:
	TextureModel(char *filename);

	bool Init(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext) override;
	void Shutdown() override;
	ID3D11ShaderResourceView *GetTexture() override;

private:

	Texture	*m_pTexture;
	char	m_textureFilename[256];

	bool LoadTexture(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext);
	void ReleaseTexture();
};

