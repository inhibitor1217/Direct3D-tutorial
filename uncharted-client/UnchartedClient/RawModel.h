#pragma once
#include "stdafx.h"
#include "Mesh.h"

class RawModel: public AlignedAllocationPolicy<16>
{
public:
	RawModel();
	RawModel(Mesh *mesh);
	RawModel(const RawModel &other);
	~RawModel();

	virtual bool Init(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext);
	virtual void Shutdown();
	virtual void Render(ID3D11DeviceContext *pDeviceContext);

	int GetIndexCount();
	Mesh *GetMesh();
	
protected:

	ID3D11Buffer	*m_pVertexBuffer;
	ID3D11Buffer	*m_pIndexBuffer;
	Mesh			*m_pMesh;

	bool InitBuffers(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext *pDeviceContext);
};