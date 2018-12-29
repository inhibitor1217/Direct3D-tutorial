#include "TextureModel.h"



TextureModel::TextureModel(char *filename)
{
	m_pTexture = nullptr;

	strncpy_s(m_Filename, filename, sizeof(m_Filename));
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


bool TextureModel::LoadModel(void *&vertices, void *&indices)
{
	m_vertexCount = 4;
	m_indexCount = 6;

	VertexType *m_vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	unsigned long *m_indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	m_vertices[0] = { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) };
	m_vertices[1] = { XMFLOAT3(-1.0f,  1.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) };
	m_vertices[2] = { XMFLOAT3( 1.0f, -1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) };
	m_vertices[3] = { XMFLOAT3( 1.0f,  1.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) };

	m_indices[0] = 0;
	m_indices[1] = 1;
	m_indices[2] = 2;
	m_indices[3] = 1;
	m_indices[4] = 3;
	m_indices[5] = 2;

	vertices = reinterpret_cast<void *>(m_vertices);
	indices = reinterpret_cast<void *>(m_indices);

	return true;
}


size_t TextureModel::GetVertexTypeSize()
{
	return sizeof(VertexType);
}


bool TextureModel::LoadTexture(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext)
{
	m_pTexture = new Texture();
	if (!m_pTexture)
		return false;

	return m_pTexture->Init(pDevice, pDeviceContext, m_Filename);
}


void TextureModel::ReleaseTexture()
{
	if (m_pTexture)
		m_pTexture->Shutdown();

	Memory::SafeDelete(m_pTexture);
}