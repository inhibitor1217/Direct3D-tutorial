#include "RawModel.h"



RawModel::RawModel()
{
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer	= nullptr;
	m_pMesh			= nullptr;
}


RawModel::RawModel(Mesh *mesh)
{
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer	= nullptr;
	m_pMesh			= mesh;
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
	return m_pMesh->GetIndexCount();
}


Mesh *RawModel::GetMesh()
{
	return m_pMesh;
}


bool RawModel::InitBuffers(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext)
{
	if (!m_pMesh)
		return false;

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = m_pMesh->GetVertexTypeSize() * m_pMesh->GetVertexCount();
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = m_pMesh->GetVertices();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	if (FAILED(pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer)))
		return false;

	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_pMesh->GetIndexCount();
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = m_pMesh->GetIndices();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	if (FAILED(pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer)))
		return false;

	return true;

}


void RawModel::ShutdownBuffers()
{
	Memory::SafeRelease(m_pIndexBuffer);
	Memory::SafeRelease(m_pVertexBuffer);
}


void RawModel::RenderBuffers(ID3D11DeviceContext *pDeviceContext)
{
	unsigned int stride = m_pMesh->GetVertexTypeSize();
	unsigned int offset = 0;

	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
