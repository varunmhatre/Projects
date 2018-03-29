#include "Entity.h"

Entity::Entity()
{
}

Entity::Entity(Mesh* m , Material* ma, XMFLOAT3 p, XMFLOAT3 r, XMFLOAT3 s)
{
	Entity_Mesh = m;
	Entity_Material = ma;
	XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(XMMatrixIdentity()));
	Position = p;
	Rotation = r;
	Scale = s;
}


Entity::~Entity()
{
}

//Set Methods
void Entity::setWorld()
{
	XMMATRIX scale = XMMatrixScaling(Scale.x, Scale.y, Scale.z);
	XMVECTOR rot = XMLoadFloat3(&Rotation);
	XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(rot);
	XMMATRIX translation = XMMatrixTranslation(Position.x, Position.y, Position.z);

	XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(scale * rotation * translation)); //order for row major
}

void Entity::setPos(XMFLOAT3 new_pos)
{
	Position = new_pos;
	setWorld();

	//XMMATRIX translation = XMMatrixTranslation(Position.x, Position.y, Position.z);
	//XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(translation));
}

void Entity::setRot(XMFLOAT3 new_rot)
{
	Rotation = new_rot;
	setWorld();
	
	//XMVECTOR rot = XMLoadFloat3(&Rotation);
	//XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(rot);
	//XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(rotation));
}

void Entity::setScale(XMFLOAT3 new_scale)
{
	Scale = new_scale;
	setWorld();
	
	//XMMATRIX scale = XMMatrixScaling(Scale.x, Scale.y, Scale.z);
	//XMStoreFloat4x4(&WorldMatrix, XMMatrixTranspose(scale));
}

//Get Metohds
XMFLOAT4X4 Entity::getWorld()
{
	return WorldMatrix;
}

XMFLOAT3 Entity::getPos()
{
	return Position;
}

XMFLOAT3 Entity::getRot()
{
	return Rotation;
}

XMFLOAT3 Entity::getScale()
{
	return Scale;
}

Mesh * Entity::getMesh()
{
	return Entity_Mesh;
}

Material * Entity::getMaterial()
{
	return Entity_Material;
}

void Entity::PrepareMaterial(XMFLOAT4X4 camViewMat, XMFLOAT4X4 camProjMat, XMFLOAT4X4 shadowViewMatrix, XMFLOAT4X4 shadowProjectionMatrix, ID3D11SamplerState* shadowSampler, ID3D11ShaderResourceView* shadowSRV)
{
	Entity_Material->GetVertShader()->SetMatrix4x4("world", getWorld());
	Entity_Material->GetVertShader()->SetMatrix4x4("view", camViewMat);
	Entity_Material->GetVertShader()->SetMatrix4x4("projection", camProjMat);
	Entity_Material->GetVertShader()->SetMatrix4x4("shadowView", shadowViewMatrix);
	Entity_Material->GetVertShader()->SetMatrix4x4("shadowProj", shadowProjectionMatrix);
	
	Entity_Material->GetPixShader()->SetSamplerState("ShadowSampler", shadowSampler);
	Entity_Material->GetPixShader()->SetShaderResourceView("ShadowMap", shadowSRV);

	Entity_Material->GetVertShader()->CopyAllBufferData();
	Entity_Material->GetPixShader()->CopyAllBufferData();

	Entity_Material->GetVertShader()->SetShader();
	Entity_Material->GetPixShader()->SetShader();
}

