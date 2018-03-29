#include "Camera.h"



Camera::Camera()
{
}

Camera::Camera(unsigned int w, unsigned int h)
{
	width = w;
	height = h;
	camPos = { 0,0,0,0 };
	camDir = { 0,0,1,0 };
	rotX = 0.0f;
	rotY = 0.0f;
	mouseDown = false;
	lastMouse.x = width/2;
	lastMouse.y = height/2;
	CreateMatrices();
}

Camera::~Camera()
{
}

void Camera::CreateMatrices()
{
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

														// Create the Projection matrix
														// - This should match the window's aspect ratio, and also update anytime
														//   the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!

	BoundingFrustum::CreateFromMatrix(frustum, P); 

	XMStoreFloat4(&camPos, pos);
}

XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

void Camera::SetMouseDown(bool m, POINT n)
{
	mouseDown = m;
	lastMouse = n;
}

void Camera::Update(POINT mouse, float deltaTime)
{
	XMVECTOR pos = XMLoadFloat4(&camPos);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);

	if (mouseDown) //FPS would look better if it's just on mouse move and not on click. It easier this way to navigate for the assignment.
	{
		float x = lastMouse.x - mouse.x;
		float y = lastMouse.y - mouse.y;

		lastMouse = mouse;

		rotY -= x / 500;
		rotX -= y / 500;
		
	}

	XMMATRIX _rotX = XMMatrixRotationX(rotX);
	XMMATRIX _rotY = XMMatrixRotationY(rotY);
	XMMATRIX rot = _rotX  * _rotY;

	XMVECTOR fwd = XMVectorSet(0, 0, 1, 0);
	fwd = XMVector3Transform(fwd, rot);

	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		fwd,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	XMVECTOR moveDir = XMVectorZero();
	float moveAmount = deltaTime;
	if (GetAsyncKeyState('W') & 0x8000)
	{
		moveDir = XMVectorSet(0, 0, 1, 0);
		moveDir = XMVector3Transform(moveDir, rot);
		moveDir = moveDir * moveAmount;
		pos += moveDir;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		moveDir = XMVectorSet(0, 0, 1, 0);
		moveDir = XMVector3Transform(moveDir, rot);
		moveDir = moveDir * moveAmount;
		pos -= moveDir;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		moveDir = XMVector3Cross(fwd, up);
		moveDir = moveDir * moveAmount;
		pos += moveDir;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		moveDir = XMVector3Cross(fwd, up);
		moveDir = moveDir * moveAmount;
		pos -= moveDir;
	}

	XMStoreFloat4(&camPos, pos);
}



void Camera::Resize(unsigned int w, unsigned int h)
{
	width = w;
	height = h;

	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!

	BoundingFrustum::CreateFromMatrix(frustum, P); 
}

BoundingFrustum Camera::GetFrustum()
{
	return frustum;
}

XMFLOAT4 Camera::GetPos()
{
	return camPos;
}

