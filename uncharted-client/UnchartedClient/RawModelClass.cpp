#include "RawModelClass.h"



RawModelClass::RawModelClass()
{
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
}


RawModelClass::RawModelClass(const RawModelClass &other)
{
}


RawModelClass::~RawModelClass()
{
}


bool RawModelClass::Init(ID3D11Device *pDevice)
{
	return InitBuffers(pDevice);
}


void RawModelClass::Shutdown()
{
	ShutdownBuffers();
}


void RawModelClass::Render(ID3D11DeviceContext *pDeviceContext)
{
	RenderBuffers(pDeviceContext);
}


int RawModelClass::GetIndexCount()
{
	return m_indexCount;
}


bool RawModelClass::InitBuffers(ID3D11Device *pDevice)
{
	m_vertexCount = 3;
	m_indexCount = 3;

	VertexType *vertices = new VertexType[m_vertexCount];
	if (!vertices)
		return false;

	unsigned long *indices = new unsigned long[m_indexCount];
	if (!indices)
		return false;

	vertices[0] = { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) };
	vertices[1] = { XMFLOAT3( 0.0f,  1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) };
	vertices[2] = { XMFLOAT3( 1.0f, -1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) };

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
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
	indexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
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


void RawModelClass::ShutdownBuffers()
{
	Memory::SafeRelease(m_pIndexBuffer);
	Memory::SafeRelease(m_pVertexBuffer);
}


void RawModelClass::RenderBuffers(ID3D11DeviceContext *pDeviceContext)
{
	unsigned int stride = sizeof(VertexType);
	unsigned int offset = 0;

	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
