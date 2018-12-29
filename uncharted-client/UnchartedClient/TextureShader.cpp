#include "TextureShader.h"



TextureShader::TextureShader()
{
	m_pSampleState = nullptr;

	m_vsFilename = "texture.vert";
	m_psFilename = "texture.pixel";
	m_vsEntryPoint = "TextureVertexShader";
	m_psEntryPoint = "TexturePixelShader";
}


bool TextureShader::Render(ID3D11DeviceContext *pDeviceContext, int indexCount,
	XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView *texture)
{
	if (!SetShaderParams(pDeviceContext, world, view, projection, texture))
		return false;

	RenderShader(pDeviceContext, indexCount);

	return true;
}


bool TextureShader::InitShader(ID3D11Device *pDevice, HWND hwnd)
{
	if (!GeneralShader::InitShader(pDevice, hwnd))
		return false;

	return LoadTextureSampler(pDevice);
}


unsigned int TextureShader::SetPolygonLayout()
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


bool TextureShader::LoadTextureSampler(ID3D11Device *pDevice)
{
	D3D11_SAMPLER_DESC samplerDesc;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	if (FAILED(pDevice->CreateSamplerState(&samplerDesc, &m_pSampleState)))
		return false;

	return true;
}


void TextureShader::ShutdownShader()
{
	GeneralShader::ShutdownShader();

	Memory::SafeRelease(m_pSampleState);
}


bool TextureShader::SetShaderParams(ID3D11DeviceContext *pDeviceContext, XMMATRIX world, XMMATRIX view, XMMATRIX projection, ID3D11ShaderResourceView *texture)
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

	pDeviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}


void TextureShader::RenderShader(ID3D11DeviceContext *pDeviceContext, int indexCount)
{
	pDeviceContext->PSSetSamplers(0, 1, &m_pSampleState);

	GeneralShader::RenderShader(pDeviceContext, indexCount);
}