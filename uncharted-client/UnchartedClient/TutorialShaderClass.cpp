#include "TutorialShaderClass.h"



TutorialShaderClass::TutorialShaderClass()
{
	m_pVertexShader = nullptr;
	m_pPixelShader	= nullptr;
	m_pLayout		= nullptr;
	m_pMatrixBuffer = nullptr;
}


TutorialShaderClass::TutorialShaderClass(const TutorialShaderClass &other)
{
}


TutorialShaderClass::~TutorialShaderClass()
{
}


bool TutorialShaderClass::Init(ID3D11Device *pDevice, HWND hwnd)
{
	return InitShader(pDevice, hwnd, "./tutorial.vert", "./tutorial.pixel");
}


void TutorialShaderClass::Shutdown()
{
	ShutdownShader();
}


bool TutorialShaderClass::Render(ID3D11DeviceContext *pDeviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
	if (!SetShaderParams(pDeviceContext, worldMatrix, viewMatrix, projectionMatrix))
		return false;

	RenderShader(pDeviceContext, indexCount);

	return true;
}


bool TutorialShaderClass::InitShader(ID3D11Device *pDevice, HWND hwnd, LPCSTR vsFilename, LPCSTR psFilename)
{
	ID3D10Blob *errorMsg = nullptr;

	ID3D10Blob *vertexShaderBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(CA2W(vsFilename), NULL, NULL, "TutorialVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMsg))) {
		if (errorMsg)
			OutputShaderErrorMsg(errorMsg, hwnd, vsFilename);
		else
			MessageBox(hwnd, vsFilename, "Missing Shader File", MB_OK);
		return false;
	}

	ID3D10Blob *pixelShaderBuffer = nullptr;
	if (FAILED(D3DCompileFromFile(CA2W(psFilename), NULL, NULL, "TutorialPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMsg))) {
		if (errorMsg)
			OutputShaderErrorMsg(errorMsg, hwnd, vsFilename);
		else
			MessageBox(hwnd, vsFilename, "Missing Shader File", MB_OK);
		return false;
	}

	if (FAILED(pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_pVertexShader)))
		return false;

	if (FAILED(pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pPixelShader)))
		return false;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	if (FAILED(pDevice->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_pLayout)))
		return false;

	Memory::SafeRelease(vertexShaderBuffer);
	Memory::SafeRelease(pixelShaderBuffer);

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	if (FAILED(pDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_pMatrixBuffer)))
		return false;

	return true;
}


void TutorialShaderClass::ShutdownShader()
{
	Memory::SafeRelease(m_pMatrixBuffer);
	Memory::SafeRelease(m_pLayout);
	Memory::SafeRelease(m_pPixelShader);
	Memory::SafeRelease(m_pVertexShader);
}


void TutorialShaderClass::OutputShaderErrorMsg(ID3D10Blob *errorMsg, HWND hwnd, LPCSTR shaderFilename)
{
	OutputDebugString(reinterpret_cast<const char *>(errorMsg->GetBufferPointer()));

	MessageBox(hwnd, "Error compiling shader.", shaderFilename, MB_OK);

	Memory::SafeRelease(errorMsg);
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


void TutorialShaderClass::RenderShader(ID3D11DeviceContext *pDeviceContext, int indexCount)
{
	pDeviceContext->IASetInputLayout(m_pLayout);

	pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
	pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

	pDeviceContext->DrawIndexed(indexCount, 0, 0);
}