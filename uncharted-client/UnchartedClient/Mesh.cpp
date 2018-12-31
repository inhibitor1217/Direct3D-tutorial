#include "Mesh.h"
#include <vector>


Mesh::Mesh()
{
}


Mesh::Mesh(const Mesh &other)
{
}


Mesh::~Mesh()
{
	Memory::SafeDeleteArray(m_vertices);
	Memory::SafeDeleteArray(m_indices);
}


size_t Mesh::GetVertexTypeSize()
{
	return sizeof(VertexType);
}


int Mesh::GetVertexCount()
{
	return m_vertexCount;
}


int Mesh::GetIndexCount()
{
	return m_indexCount;
}


void *Mesh::GetVertices()
{
	return reinterpret_cast<void *>(m_vertices);
}


void *Mesh::GetIndices()
{
	return reinterpret_cast<void *>(m_indices);
}


bool Mesh::LoadOBJ(char *filename)
{
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<XMFLOAT3> vertices;
	std::vector<XMFLOAT2> uvs;
	std::vector<XMFLOAT3> normals;

	FILE *file;
	char buffer[128];

	if (fopen_s(&file, filename, "r") != 0)
		return false;
	while (fscanf_s(file, "%s", buffer, sizeof(buffer)) != EOF) {
		if (strcmp(buffer, "v") == 0) {
			XMFLOAT3 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertices.push_back(vertex);
		}
		else if (strcmp(buffer, "vt") == 0) {
			XMFLOAT2 uv;
			fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
			uvs.push_back(uv);
		}
		else if (strcmp(buffer, "vn") == 0) {
			XMFLOAT3 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);
		}
		else if (strcmp(buffer, "f") == 0) {
			unsigned int vIdx[3], uIdx[3], nIdx[3];
			fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				vIdx, uIdx, nIdx, vIdx + 1, uIdx + 1, nIdx + 1, vIdx + 2, uIdx + 2, nIdx + 2);
			for (int i = 0; i < 3; i++) {
				vertexIndices.push_back(vIdx[i]);
				uvIndices.push_back(uIdx[i]);
				normalIndices.push_back(nIdx[i]);
			}
		}
	}

	if (fclose(file) != 0)
		return false;

	m_vertexCount = 3 * vertexIndices.size();
	m_indexCount = vertexIndices.size();

	m_vertices = new VertexType[m_vertexCount];
	m_indices = new unsigned int[m_indexCount];

	for (int i = 0; i < m_indexCount; i++) {
		m_indices[i] = i;
		m_vertices[i].position = vertices[vertexIndices[i] - 1];
		m_vertices[i].uvCoord = uvs[uvIndices[i] - 1];
		m_vertices[i].normal = normals[normalIndices[i] - 1];
	}

	return true;
}