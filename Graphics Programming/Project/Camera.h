#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Vertex.h"
#include <DirectXCollision.h>

using namespace DirectX;

class Camera
{
public:
	Camera();
	Camera(unsigned int, unsigned int);
	~Camera();
	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();
	void SetMouseDown(bool, POINT);
	void Update(POINT, float);
	void Resize(unsigned int width, unsigned int height);
	BoundingFrustum GetFrustum();
	XMFLOAT4 GetPos();

private:
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;

	unsigned int width;
	unsigned int height;
	void CreateMatrices();

	XMFLOAT4 camPos;
	XMVECTOR camDir;
	float rotX;
	float rotY;
	POINT lastMouse;
	bool mouseDown;

	BoundingFrustum frustum;
};

