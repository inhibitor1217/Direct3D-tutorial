#include "TextShader.h"



TextShader::TextShader()
{
	m_psFilename = "text.pixel";
	m_psEntryPoint = "TextPixelShader";
}


size_t TextShader::GetPSUniformVariableSize()
{
	return sizeof(PSUniformVariableType);
}



void *TextShader::CreatePSUniformVariable(XMFLOAT4 color)
{
	PSUniformVariableType *ptr = reinterpret_cast<PSUniformVariableType *>(malloc(sizeof(PSUniformVariableType)));

	ptr->color = color;

	return reinterpret_cast<void *>(ptr);
}


bool TextShader::LoadTextureSampler(ID3D11Device *pDevice) {

	D3D11_SAMPLER_DESC samplerDesc;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
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