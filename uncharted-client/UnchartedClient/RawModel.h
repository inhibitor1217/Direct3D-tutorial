#pragma once
#include "stdafx.h"

class RawModel: public AlignedAllocationPolicy<16>
{
public:
	RawModel();
	RawModel(const RawModel &other);
	~RawModel();

	virtual bool Init(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext);
	virtual void Shutdown();
	virtual void Render(ID3D11DeviceContext *pDeviceContext);
	virtual ID3D11ShaderResourceView *GetTexture() = 0;

	int GetIndexCount();
	
protected:
	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	ID3D11Buffer *m_pVertexBuffer;
	ID3D11Buffer *m_pIndexBuffer;
	int m_vertexCount = 0;
	int m_indexCount = 0;

	bool InitBuffers(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext);
	virtual bool LoadModel(void *&vertices, void *&indices);
	virtual size_t GetVertexTypeSize();
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext *pDeviceContext);
};

