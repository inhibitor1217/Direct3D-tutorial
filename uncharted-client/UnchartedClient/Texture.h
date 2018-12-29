#pragma once
#include "stdafx.h"

class Texture
{
public:
	Texture();
	Texture(const Texture &other);
	~Texture();

	bool Init(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext, char *filename);
	void Shutdown();

	ID3D11ShaderResourceView *GetTexture();
	
protected:
	struct TargaHeader {
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

	struct BMPHeader {
		unsigned char data[0x7A];

		int getWidth() {
			return *((int *)(data + 0x12));
		}

		int getHeight() {
			return *((int *)(data + 0x16));
		}

		unsigned short getBPP() {
			return *((unsigned short *)(data + 0x1C));
		}
	};

	unsigned char				*m_pTextureData;
	ID3D11Texture2D				*m_pTexture;
	ID3D11ShaderResourceView	*m_pTextureView;

	bool LoadTarga(char *filename, int &width, int &height);
	bool LoadBMPARGB32(char *filename, int &width, int &height);
};

