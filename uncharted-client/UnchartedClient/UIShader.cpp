#include "UIShader.h"



UIShader::UIShader()
{
	m_vsFilename = "ui.vert";
	m_psFilename = "ui.pixel";
	m_vsEntryPoint = "UIVertexShader";
	m_psEntryPoint = "UIPixelShader";
}


unsigned int UIShader::SetPolygonLayout()
{
	m_pPolygonLayout = new D3D11_INPUT_ELEMENT_DESC[2];

	m_pPolygonLayout[0].SemanticName = "POSITION";
	m_pPolygonLayout[0].SemanticIndex = 0;
	m_pPolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	m_pPolygonLayout[0].InputSlot = 0;
	m_pPolygonLayout[0].AlignedByteOffset = 0;
	m_pPolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	m_pPolygonLayout[0].InstanceDataStepRate = 0;

	m_pPolygonLayout[1].SemanticName = "TEXCOORD";
	m_pPolygonLayout[1].SemanticIndex = 0;
	m_pPolygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	m_pPolygonLayout[1].InputSlot = 0;
	m_pPolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	m_pPolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	m_pPolygonLayout[1].InstanceDataStepRate = 0;

	return 2;
}