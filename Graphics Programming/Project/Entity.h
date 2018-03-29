#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Vertex.h"
#include "Mesh.h"
#include "Material.h"

using namespace DirectX;

class Entity
{
public:
	Entity();
	Entity(Mesh*, Material*, XMFLOAT3, XMFLOAT3, XMFLOAT3);
	~Entity();
	
	//Set Methods
	void setWorld();
	void setPos(XMFLOAT3);
	void setRot(XMFLOAT3);
	void setScale(XMFLOAT3);

	//Get Methods
	XMFLOAT4X4 getWorld();
	XMFLOAT3 getPos();
	XMFLOAT3 getRot();
	XMFLOAT3 getScale();
	Mesh* getMesh();
	Material* getMaterial();
	void PrepareMaterial(XMFLOAT4X4, XMFLOAT4X4, XMFLOAT4X4, XMFLOAT4X4, ID3D11SamplerState*, ID3D11ShaderResourceView*);

private:
	Mesh* Entity_Mesh;
	XMFLOAT4X4 WorldMatrix;

	//Entity Vectors
	XMFLOAT3 Position;
	XMFLOAT3 Rotation;
	XMFLOAT3 Scale;

	Material* Entity_Material;
};

