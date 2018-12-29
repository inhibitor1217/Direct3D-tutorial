#include "RawModel.h"



RawModel::RawModel()
{
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
}


RawModel::RawModel(const RawModel &other)
{
}


RawModel::~RawModel()
{
}


bool RawModel::Init(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext)
{
	return InitBuffers(pDevice, pDeviceContext);
}


void RawModel::Shutdown()
{
	ShutdownBuffers();
}


void RawModel::Render(ID3D11DeviceContext *pDeviceContext)
{
	RenderBuffers(pDeviceContext);
}


int RawModel::GetIndexCount()
{
	return m_indexCount;
}


bool RawModel::InitBuffers(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext)
{
	void *vertices, *indices;
	LoadModel(vertices, indices);

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = GetVertexTypeSize() * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer)))
		return false;

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


size_t RawModel::GetVertexTypeSize()
{
	return sizeof(VertexType);
}


bool RawModel::LoadModel(void *&vertices, void *&indices)
{
	m_vertexCount = 3;
	m_indexCount = 3;

	VertexType *m_vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	unsigned long *m_indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	m_vertices[0] = { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) };
	m_vertices[1] = { XMFLOAT3(0.0f,  1.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) };
	m_vertices[2] = { XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) };

	m_indices[0] = 0;
	m_indices[1] = 1;
	m_indices[2] = 2;

	vertices = reinterpret_cast<void *>(m_vertices);
	indices = reinterpret_cast<void *>(m_indices);

	return true;
}


void RawModel::ShutdownBuffers()
{
	Memory::SafeRelease(m_pIndexBuffer);
	Memory::SafeRelease(m_pVertexBuffer);
}


void RawModel::RenderBuffers(ID3D11DeviceContext *pDeviceContext)
{
	unsigned int stride = GetVertexTypeSize();
	unsigned int offset = 0;

	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
