#include "GeneralShader.h"



GeneralShader::GeneralShader()
{
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	m_pLayout = nullptr;
	m_pUniformVariableBuffer = nullptr;
	m_pPolygonLayout = nullptr;

	m_vsFilename = "";
	m_psFilename = "";
	m_vsEntryPoint = "";
	m_psEntryPoint = "";
}


GeneralShader::GeneralShader(const GeneralShader &other)
{
}


GeneralShader::~GeneralShader()
{
}


bool GeneralShader::Init(ID3D11Device *pDevice, HWND hwnd)
{
	return InitShader(pDevice, hwnd);
}


void GeneralShader::Shutdown()
{
	ShutdownShader();
}


bool GeneralShader::Render(ID3D11DeviceContext *pDeviceContext, int indexCount,
	void *pUniformVariables, ID3D11ShaderResourceView *texture)
{
	if (!SetShaderParams(pDeviceContext, pUniformVariables, texture))
		return false;

	RenderShader(pDeviceContext, indexCount);

	return true;
}


bool GeneralShader::InitShader(ID3D11Device *pDevice, HWND hwnd)
{
	ID3D10Blob *errorMsg = nullptr;

	ID3D10Blob *vertexShaderBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(CA2W(m_vsFilename), NULL, NULL, m_vsEntryPoint, "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMsg))) {
		if (errorMsg)
			OutputShaderErrorMsg(errorMsg, hwnd, m_vsFilename);
		else
			MessageBox(hwnd, m_vsFilename, "Missing Shader File", MB_OK);
		return false;
	}

	ID3D10Blob *pixelShaderBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(CA2W(m_psFilename), NULL, NULL, m_psEntryPoint, "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMsg))) {
		if (errorMsg)
			OutputShaderErrorMsg(errorMsg, hwnd, m_psFilename);
		else
			MessageBox(hwnd, m_psFilename, "Missing Shader File", MB_OK);
		return false;
	}

	if (FAILED(pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader)))
		return false;

	if (FAILED(pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader)))
		return false;

	unsigned int numElements = SetPolygonLayout();
	if (!m_pPolygonLayout)
		return false;

	if (FAILED(pDevice->CreateInputLayout(m_pPolygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pLayout)))
		return false;

	Memory::SafeRelease(vertexShaderBuffer);
	Memory::SafeRelease(pixelShaderBuffer);
	Memory::SafeDeleteArray(m_pPolygonLayout);

	D3D11_BUFFER_DESC uniformVariableBufferDesc;
	uniformVariableBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	uniformVariableBufferDesc.ByteWidth = GetUniformVariableSize();
	uniformVariableBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	uniformVariableBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	uniformVariableBufferDesc.MiscFlags = 0;
	uniformVariableBufferDesc.StructureByteStride = 0;

	if (FAILED(pDevice->CreateBuffer(&uniformVariableBufferDesc, NULL, &m_pUniformVariableBuffer)))
		return false;

	return true;
}

void GeneralShader::ShutdownShader()
{
	Memory::SafeRelease(m_pUniformVariableBuffer);
	Memory::SafeRelease(m_pLayout);
	Memory::SafeRelease(m_pPixelShader);
	Memory::SafeRelease(m_pVertexShader);
	Memory::SafeDeleteArray(m_pPolygonLayout);
}


void GeneralShader::OutputShaderErrorMsg(ID3D10Blob *errorMsg, HWND hwnd, LPCSTR shaderFilename)
{
	OutputDebugString(reinterpret_cast<const char *>(errorMsg->GetBufferPointer()));
	MessageBox(hwnd, "Error compiling shader.", shaderFilename, MB_OK);
	Memory::SafeRelease(errorMsg);
}


void GeneralShader::RenderShader(ID3D11DeviceContext *pDeviceContext, int indexCount)
{
	pDeviceContext->IASetInputLayout(m_pLayout);

	pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	pDeviceContext->DrawIndexed(indexCount, 0, 0);
}


size_t GeneralShader::GetUniformVariableSize()
{
	return sizeof(UniformVariableType);
}


void *GeneralShader::CreateUniformVariable(XMMATRIX world, XMMATRIX view, XMMATRIX projection)
{
	UniformVariableType *ptr = reinterpret_cast<UniformVariableType *>(malloc(sizeof(UniformVariableType)));

	ptr->world = world;
	ptr->view = view;
	ptr->projection = projection;

	return reinterpret_cast<void *>(ptr);
}


bool GeneralShader::SetShaderParams(ID3D11DeviceContext *pDeviceContext, void *pUniformVariables, ID3D11ShaderResourceView *texture)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(pDeviceContext->Map(m_pUniformVariableBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
		return false;

	memcpy_s(mappedResource.pData, GetUniformVariableSize(), pUniformVariables, GetUniformVariableSize());

	pDeviceContext->Unmap(m_pUniformVariableBuffer, 0);

	pDeviceContext->VSSetConstantBuffers(0, 1, &m_pUniformVariableBuffer);

	pDeviceContext->PSSetShaderResources(0, 1, &texture);

	return true;
}