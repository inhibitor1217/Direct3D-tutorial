#include "UIModel.h"



UIModel::UIModel(char *filename)
{
	m_pTexture = nullptr;

	strncpy_s(m_textureFilename, filename, sizeof(m_textureFilename));
}


UIModel::UIModel(const UIModel &other)
{
}


UIModel::~UIModel()
{
}


bool UIModel::Init(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext, int screenWidth, int screenHeight, int width, int height)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	m_width = width;
	m_height = height;
	m_prevPosX = -1;
	m_prevPosY = -1;

	if (!InitBuffers(pDevice))
		return false;

	return LoadTexture(pDevice, pDeviceContext);
}


void UIModel::Shutdown()
{
	ReleaseTexture();

	ShutdownBuffers();
}


bool UIModel::Render(ID3D11DeviceContext *pDeviceContext, int posX, int posY)
{
	if (!UpdateBuffers(pDeviceContext, posX, posY))
		return false;

	RenderBuffers(pDeviceContext);
	return true;
}


int UIModel::GetIndexCount()
{
	return m_indexCount;
}


ID3D11ShaderResourceView *UIModel::GetTexture()
{
	return m_pTexture->GetTexture();
}


bool UIModel::InitBuffers(ID3D11Device *pDevice)
{
	m_vertexCount = 4;
	m_indexCount = 6;

	VertexType *vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	memset(vertices, 0, sizeof(VertexType) * m_vertexCount);

	unsigned long *indices = new unsigned long[m_indexCount];
	if (!indices) {
		Memory::SafeDeleteArray(vertices);
		return false;
	}

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 3;
	indices[5] = 1;

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer))) {
		Memory::SafeDeleteArray(vertices);
		Memory::SafeDeleteArray(indices);
		return false;
	}

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer)))
		return false;

	Memory::SafeDeleteArray(vertices);
	Memory::SafeDeleteArray(indices);

	return true;

}


void UIModel::ShutdownBuffers()
{
	Memory::SafeRelease(m_pVertexBuffer);
	Memory::SafeRelease(m_pIndexBuffer);
}


bool UIModel::UpdateBuffers(ID3D11DeviceContext *pDeviceContext, int posX, int posY)
{
	if (posX == m_prevPosX && posY == m_prevPosY)
		return true;

	m_prevPosX = posX;
	m_prevPosY = posY;

	float left = static_cast<float>(posX - m_screenWidth / 2);
	float right = left + static_cast<float>(m_width);
	float top = static_cast<float>(m_screenHeight / 2 - posY);
	float bottom = top - static_cast<float>(m_height);

	VertexType *vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	vertices[0] = { XMFLOAT3(  left,    top, 0.0f), XMFLOAT2(0.0f, 0.0f) };
	vertices[1] = { XMFLOAT3( right, bottom, 0.0f), XMFLOAT2(1.0f, 1.0f) };
	vertices[2] = { XMFLOAT3(  left, bottom, 0.0f), XMFLOAT2(0.0f, 1.0f) };
	vertices[3] = { XMFLOAT3( right,    top, 0.0f), XMFLOAT2(1.0f, 0.0f) };

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))) {
		Memory::SafeDeleteArray(vertices);
		return false;
	}

	VertexType *pVertices = reinterpret_cast<VertexType *>(mappedResource.pData);
	memcpy(pVertices, reinterpret_cast<void *>(vertices), sizeof(VertexType) * m_vertexCount);

	pDeviceContext->Unmap(m_pVertexBuffer, 0);

	Memory::SafeDelete(vertices);

	return true;
}


void UIModel::RenderBuffers(ID3D11DeviceContext *pDeviceContext)
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool UIModel::LoadTexture(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext)
{
	m_pTexture = new Texture();
	if (!m_pTexture)
		return false;

	return m_pTexture->Init(pDevice, pDeviceContext, m_textureFilename);
}


void UIModel::ReleaseTexture()
{
	if (m_pTexture)
		m_pTexture->Shutdown();

	Memory::SafeDelete(m_pTexture);
}