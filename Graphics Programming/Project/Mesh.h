#pragma once

#include <d3d11.h>
#include "Vertex.h"
#include <DirectXCollision.h>
#include <vector>

using namespace DirectX;

class Mesh
{
public:
	Mesh();
	~Mesh();

	Mesh(Vertex*, int, UINT*, int, ID3D11Device*, std::string);
	Mesh(ID3D11Device*, char*, std::string);


	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();

	Vertex * GetVertices();
	UINT * GetFaces();
	BoundingOrientedBox getOBB();
	std::string GetName();

private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int num_indices;
	Vertex* vertices;
	void Geometry(Vertex * vertices, ID3D11Device * device, UINT * indices, int num_ver);

	void CreateBuffers(Vertex * vertArray, int numVerts, unsigned int * indexArray, int numIndices, ID3D11Device * device);

	void CalculateTangents(Vertex * verts, int numVerts, unsigned int * indices, int numIndices);
	BoundingOrientedBox obb;
	std::vector<Vertex> verts;
	std::vector<UINT> indices;
	std::string name;
};

