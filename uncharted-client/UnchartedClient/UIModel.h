#pragma once
#include "stdafx.h"
#include "Texture.h"

class UIModel
{
public:
	UIModel();
	UIModel(const UIModel &other);
	~UIModel();

	bool Init(ID3D11Device *pDevice, ID3D11DeviceContext *pDeviceContext,
		int screenWidth, int screenHeight, int width, int height);
	void Shutdown();
	bool Render(ID3D11DeviceContext *pDeviceContext, int posX, int posY);

	int GetIndexCount();
	ID3D11ShaderResourceView *GetTexture();
	void SetTexture(Texture *texture);

protected:
	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

	Texture *m_pTexture;

	ID3D11Buffer *m_pVertexBuffer = nullptr;
	ID3D11Buffer *m_pIndexBuffer = nullptr;

	int m_vertexCount	= 0;
	int m_indexCount	= 0;
	int m_screenWidth	= 0;
	int m_screenHeight	= 0;
	int m_width			= 0;
	int m_height		= 0;
	int m_prevPosX		= 0;
	int m_prevPosY		= 0;

	virtual bool InitBuffers(ID3D11Device *pDevice);
	virtual void ShutdownBuffers();
	virtual bool UpdateBuffers(ID3D11DeviceContext *pDeviceContext, int posX, int posY);
	virtual void RenderBuffers(ID3D11DeviceContext *pDeviceContext);
};

