#include "Texture.h"



Texture::Texture(char *filename)
{
	m_pTextureData	= nullptr;
	m_pTexture		= nullptr;
	m_pTextureView	= nullptr;

	strncpy_s(m_Filename, filename, sizeof(m_Filename));
}


Texture::Texture(const Texture &other)
{
}


Texture::~Texture()
{
}



bool Texture::Init(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext)
{
	int width = 0;
	int height = 0;

	if (strstr(m_Filename, ".bmp") != NULL) {
		if (!LoadBMPARGB32(width, height))
			return false;
	}
	else if (strstr(m_Filename, ".tga") != NULL) {
		if (!LoadTarga(width, height))
			return false;
	}
	else if (strstr(m_Filename, ".png") != NULL) {
		if (!LoadPNG(width, height))
			return false;
	}
	else
		return false;

	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	if (FAILED(pDevice->CreateTexture2D(&textureDesc, NULL, &m_pTexture)))
		return false;

	UINT rowPitch = (width * 4) * sizeof(unsigned char);

	pDeviceContext->UpdateSubresource(m_pTexture, 0, NULL, m_pTextureData, rowPitch, 0);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	if (FAILED(pDevice->CreateShaderResourceView(m_pTexture, &srvDesc, &m_pTextureView)))
		return false;

	pDeviceContext->GenerateMips(m_pTextureView);

	Memory::SafeDeleteArray(m_pTextureData);

	return true;
}


void Texture::Shutdown()
{
	Memory::SafeRelease(m_pTextureView);
	Memory::SafeRelease(m_pTexture);
	Memory::SafeDeleteArray(m_pTextureData);
}


ID3D11ShaderResourceView *Texture::GetTexture()
{
	return m_pTextureView;
}


bool Texture::LoadPNG(int &width, int &height)
{
	std::vector<unsigned char> image;
	unsigned int tWidth, tHeight;

	unsigned int error = lodepng::decode(image, tWidth, tHeight, reinterpret_cast<const char *>(m_Filename));

	if (error)
		return false;

	width = static_cast<int>(tWidth);
	height = static_cast<int>(tHeight);

	m_pTextureData = new unsigned char[image.size()];
	for (unsigned int i = 0; i < image.size(); i++)
		m_pTextureData[i] = image[i];

	return true;
}


bool Texture::LoadBMPARGB32(int &width, int &height)
{
	FILE *file;
	if (fopen_s(&file, m_Filename, "rb") != 0)
		return false;

	BMPHeader bmpFileHeader;
	unsigned int count = (unsigned int)fread(&bmpFileHeader, sizeof(BMPHeader), 1, file);
	if (count != 1)
		return false;

	width = bmpFileHeader.getWidth();
	height = bmpFileHeader.getHeight();

	int bpp = static_cast<int>(bmpFileHeader.getBPP());
	if (bpp != 32)
		return false;

	int imageBufferSize = width * height * 4;
	unsigned char *bmpImage = new unsigned char[imageBufferSize];
	if (!bmpImage)
		return false;

	count = (unsigned int)fread(bmpImage, 1, imageBufferSize, file);
	if (count != imageBufferSize)
		return false;

	if (fclose(file) != 0)
		return false;

	m_pTextureData = new unsigned char[imageBufferSize];
	if (!m_pTextureData)
		return false;

	int index = 0, bmpIndex = 4 * width *  (height - 1);
	for (int i = 0; i < height; i++, bmpIndex -= 8 * width) {
		for (int j = 0; j < width; j++, index += 4, bmpIndex += 4) {
			m_pTextureData[index + 0] = bmpImage[bmpIndex + 2];
			m_pTextureData[index + 1] = bmpImage[bmpIndex + 1];
			m_pTextureData[index + 2] = bmpImage[bmpIndex + 0];
			m_pTextureData[index + 3] = bmpImage[bmpIndex + 3];
		}
	}

	Memory::SafeDeleteArray(bmpImage);

	return true;
}


bool Texture::LoadTarga(int &width, int &height)
{
	FILE *file;
	if (fopen_s(&file, m_Filename, "rb") != 0)
		return false;

	TargaHeader targaFileHeader;
	unsigned int count = (unsigned int)fread(&targaFileHeader, sizeof(TargaHeader), 1, file);
	if (count != 1)
		return false;

	width = static_cast<int>(targaFileHeader.width);
	height = static_cast<int>(targaFileHeader.height);
	
	int bpp = static_cast<int>(targaFileHeader.bpp);
	if (bpp != 32)
		return false;

	int imageBufferSize = width * height * 4;
	unsigned char *targaImage = new unsigned char[imageBufferSize];
	if (!targaImage)
		return false;

	count = (unsigned int)fread(targaImage, 1, imageBufferSize, file);
	if (count != imageBufferSize)
		return false;

	if (fclose(file) != 0)
		return false;

	m_pTextureData = new unsigned char[imageBufferSize];
	if (!m_pTextureData)
		return false;

	int index = 0;
	int k = (width * height * 4) - (width * 4);

	for (int j = 0; j < height; j++, k -= (width * 8))
		for (int i = 0; i < width; i++, k += 4, index += 4) {
			m_pTextureData[index + 0] = targaImage[k + 2];
			m_pTextureData[index + 1] = targaImage[k + 1];
			m_pTextureData[index + 2] = targaImage[k + 0];
			m_pTextureData[index + 3] = targaImage[k + 3];
		}

	Memory::SafeDeleteArray(targaImage);

	return true;

}