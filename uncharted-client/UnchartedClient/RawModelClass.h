#pragma once
#include "stdafx.h"

class RawModelClass: public AlignedAllocationPolicy<16>
{
public:
	RawModelClass();
	RawModelClass(const RawModelClass &other);
	~RawModelClass();

	bool Init(ID3D11Device *pDevice);
	void Shutdown();
	void Render(ID3D11DeviceContext *pDeviceContext);

	int GetIndexCount();
	
private:
	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	ID3D11Buffer *m_pVertexBuffer;
	ID3D11Buffer *m_pIndexBuffer;
	int m_vertexCount = 0;
	int m_indexCount = 0;

	bool InitBuffers(ID3D11Device *pDevice);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext *pDeviceContext);
};

