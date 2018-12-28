#include "TutorialShaderClass.h"


TutorialShaderClass::TutorialShaderClass()
{
	m_vsFilename = "tutorial.vert";
	m_psFilename = "tutorial.pixel";
	m_vsEntryPoint = "TutorialVertexShader";
	m_psEntryPoint = "TutorialPixelShader";
}


bool TutorialShaderClass::Render(ID3D11DeviceContext *pDeviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	if (!SetShaderParams(pDeviceContext, worldMatrix, viewMatrix, projectionMatrix))
		return false;

	RenderShader(pDeviceContext, indexCount);

	return true;
}


unsigned int TutorialShaderClass::SetPolygonLayout()
{
	m_pPolygonLayout = new D3D11_INPUT_ELEMENT_DESC[2];

	m_pPolygonLayout[0].SemanticName = "POSITION";
	m_pPolygonLayout[0].SemanticIndex = 0;
	m_pPolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	m_pPolygonLayout[0].InputSlot = 0;
	m_pPolygonLayout[0].AlignedByteOffset = 0;
	m_pPolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	m_pPolygonLayout[0].InstanceDataStepRate = 0;

	m_pPolygonLayout[1].SemanticName = "COLOR";
	m_pPolygonLayout[1].SemanticIndex = 0;
	m_pPolygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	m_pPolygonLayout[1].InputSlot = 0;
	m_pPolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	m_pPolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	m_pPolygonLayout[1].InstanceDataStepRate = 0;

	return 2;
}


bool TutorialShaderClass::SetShaderParams(ID3D11DeviceContext *pDeviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection)
{
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	projection = XMMatrixTranspose(projection);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(pDeviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	MatrixBufferType *dataPtr = reinterpret_cast<MatrixBufferType *>(mappedResource.pData);

	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = projection;

	pDeviceContext->Unmap(m_pMatrixBuffer, 0);

	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pMatrixBuffer);

	return true;
}