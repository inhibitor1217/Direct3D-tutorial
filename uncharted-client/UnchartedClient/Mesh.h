#pragma once
#include "stdafx.h"

class Mesh
{
public:
	Mesh();
	Mesh(const Mesh &other);
	~Mesh();

	bool LoadOBJ(char *filename);

	virtual size_t GetVertexTypeSize();
	int GetVertexCount();
	int GetIndexCount();
	void *GetVertices();
	void *GetIndices();

private:
	struct VertexType {
		XMFLOAT3 position;
		XMFLOAT2 uvCoord;
		XMFLOAT3 normal;
	};

	int m_vertexCount = 0;
	int m_indexCount = 0;
	VertexType *m_vertices = nullptr;
	unsigned int *m_indices = nullptr;

};

