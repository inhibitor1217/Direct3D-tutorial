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
	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	Texture	*m_pTexture;
	char	m_Filename[256];

	bool LoadModel(void *&vertices, void *&indices) override;
	size_t GetVertexTypeSize() override;
	bool LoadTexture(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext);
	void ReleaseTexture();
};

