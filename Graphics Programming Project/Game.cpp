#include "Game.h"
#include "Vertex.h"
#include <DirectXCollision.h>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore( 
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexBuffer = 0;
	indexBuffer = 0;
	vertexShader = 0;
	pixelShader = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.");
#endif
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Release any (and all!) DirectX objects
	// we've made in the Game class
	//if (vertexBuffer) { vertexBuffer->Release(); }
	//if (indexBuffer) { indexBuffer->Release(); }

	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;

	delete figure1;
	delete figure2;
	delete figure3;

	for (int i = 0; i < 6; i++)
	{
		delete en[i];
	}

	delete cube;
	delete cone;
	delete sphere;
	delete helix;
	delete torus;
	delete cylinder;

	delete m1;
	delete m2;

	delete camera;

	sampState->Release();
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	LoadShaders();

	CreateWICTextureFromFile(device, context, L"Debug/Assets/Images/Textures/crate.png", 0, &crateSRV);
	CreateWICTextureFromFile(device, context, L"Debug/Assets/Images/Textures/marble.png", 0, &marbleSRV);

	D3D11_SAMPLER_DESC samDesc = {};

	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samDesc, &sampState);

	m1 = new Material(pixelShader, vertexShader, crateSRV, sampState);
	m2 = new Material(pixelShader, vertexShader, marbleSRV, sampState);

	CreateBasicGeometry();

	camera = new Camera(width, height);

	Lights::DirectionalLight dirL;

	dirL.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1);
	dirL.DiffuseColor = XMFLOAT4(1, 0, 0, 1);
	dirL.Direction = XMFLOAT3(1, 1, 0);

	pixelShader->SetData(
		"dirLight",  // The name of the (eventual) variable in the shader
		&dirL,   // The address of the data to copy
		sizeof(Lights::DirectionalLight)); // The size of the data to copy

	Lights::DirectionalLight dirLDos;

	dirLDos.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1);
	dirLDos.DiffuseColor = XMFLOAT4(1, 1, 1, 1);
	dirLDos.Direction = XMFLOAT3(0, -1, 1); 

	pixelShader->SetData(
		"dirLight2",  
		&dirLDos,   
		sizeof(Lights::DirectionalLight)); 

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	if (!vertexShader->LoadShaderFile(L"Debug/VertexShader.cso"))
		vertexShader->LoadShaderFile(L"VertexShader.cso");		

	pixelShader = new SimplePixelShader(device, context);
	if(!pixelShader->LoadShaderFile(L"Debug/PixelShader.cso"))	
		pixelShader->LoadShaderFile(L"PixelShader.cso");

}



// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red	= XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green	= XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue   = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 cyan	= XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
	XMFLOAT4 pink   = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	Vertex vertices1[] = 
	{
		{ XMFLOAT3(+0.0f, +1.0f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0)},
		{ XMFLOAT3(+1.0f, -1.0f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(-1.0f, -1.0f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	UINT indices1[] = { 0, 1, 2 };

	figure1 = new Mesh(vertices1, 3, indices1, 3, device, "mesh1");

	Vertex vertices2[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(-1.0f, +1.0f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(+1.0f, +1.0f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(+1.0f, -1.0f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
	};

	UINT indices2[] = { 0, 1, 2, 2, 3,  0 };

	figure2 = new Mesh(vertices2, 4, indices2, 6, device, "mesh2");

	Vertex vertices3[] =
	{
		{ XMFLOAT3(-2.0f, +0.0f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(-1.0f, +1.0f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(+0.0f, +0.0f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(-0.5f, -1.0f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(-1.5f, -1.0f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
	};

	UINT indices3[] = { 0, 1, 2, 2, 4, 0, 2, 3, 4 };

	figure3 = new Mesh(vertices3, 5, indices3, 9, device, "mesh3");

	cone = new Mesh(device, "Debug/Assets/Models/Textures/cone.obj", "cone");
	helix = new Mesh(device, "Debug/Assets/Models/Textures/helix.obj", "helix");
	sphere = new Mesh(device, "Debug/Assets/Models/Textures/sphere.obj", "sphere");
	cube = new Mesh(device, "Debug/Assets/Models/Textures/cube.obj", "cube");
	cylinder = new Mesh(device, "Debug/Assets/Models/Textures/cylinder.obj", "cylinder");
	torus = new Mesh(device, "Debug/Assets/Models/Textures/torus.obj", "torus");

	en[0] = new Entity(cone, m1, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));
	en[1] = new Entity(torus, m1, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));
	en[2] = new Entity(cube, m1, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));
	en[3] = new Entity(cylinder, m1, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));
	en[4] = new Entity(helix, m1, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));
	en[5] = new Entity(sphere, m1, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));

	en[0]->setScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	//en[0]->setPos(XMFLOAT3(sinTime, sinTime, sinTime));
	//en[0]->setRot(XMFLOAT3(0, 0, totalTime));

	en[1]->setScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	//en[1]->setPos(XMFLOAT3(sinTime, -sinTime, -sinTime));
	//en[1]->setRot(XMFLOAT3(0, 0, totalTime));

	en[2]->setScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	//en[2]->setRot(XMFLOAT3(0, 0, 2 * totalTime));

	en[3]->setScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	//en[3]->setPos(XMFLOAT3(sinTime / 2, -cosTime / 2, 0));
	//en[3]->setRot(XMFLOAT3(0, 0, totalTime * 4));

	en[4]->setScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	//en[4]->setPos(XMFLOAT3(-sinTime / 2, cosTime / 2, 0));
	//en[4]->setRot(XMFLOAT3(0, 0, totalTime * 4));

	en[5]->setScale(XMFLOAT3(0.5f, 0.5f, 0.5f));
	//en[5]->setPos(XMFLOAT3(1, 1, 0));

	en[0]->setPos(XMFLOAT3(1, 1, 0));
	en[1]->setPos(XMFLOAT3(1, -1, 0));
	en[2]->setPos(XMFLOAT3(-1, -1, 0));
	en[3]->setPos(XMFLOAT3(-1, 1, 0));
	en[5]->setPos(XMFLOAT3(1, 0, 0));
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	camera->Resize(width, height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	
	float sinTime2 = sin(totalTime);
	float sinTime = sin(totalTime * 2);
	float cosTime = cos(totalTime * 2);

	
	camera->Update(prevMousePos, deltaTime);

}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = {0.4f, 0.6f, 0.75f, 0.0f};

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView, 
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	//------------------------------------------------------------------------



	//------------------------------------------------------------------------

	//Set m1 as material for next objects
	m1->SetShaderResourceView();
	m1->SetSamplerState();

	en[0]->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix());

	stride = sizeof(Vertex);
	offset = 0;

	vertexBuffer = en[0]->getMesh()->GetVertexBuffer();
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(en[0]->getMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(
		en[0]->getMesh()->GetIndexCount(),
		0,     
		0);    
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------

	en[1]->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix());

	stride = sizeof(Vertex);
	offset = 0;

	vertexBuffer = en[1]->getMesh()->GetVertexBuffer();
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(en[1]->getMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(
		en[1]->getMesh()->GetIndexCount(),
		0,
		0);
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------

	en[2]->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix());

	stride = sizeof(Vertex);
	offset = 0;

	vertexBuffer = en[2]->getMesh()->GetVertexBuffer();
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(en[2]->getMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(
		en[2]->getMesh()->GetIndexCount(),
		0,
		0);
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------

	//Set m2 as material for next objects
	m2->SetShaderResourceView();
	m2->SetSamplerState();

	en[3]->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix());

	stride = sizeof(Vertex);
	offset = 0;

	vertexBuffer = en[3]->getMesh()->GetVertexBuffer();
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(en[3]->getMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(
		en[3]->getMesh()->GetIndexCount(),
		0,
		0);
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------

	en[4]->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix());

	stride = sizeof(Vertex);
	offset = 0;

	vertexBuffer = en[4]->getMesh()->GetVertexBuffer();
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(en[4]->getMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(
		en[4]->getMesh()->GetIndexCount(),
		0,
		0);
	//------------------------------------------------------------------------
	//------------------------------------------------------------------------

	en[5]->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix());

	stride = sizeof(Vertex);
	offset = 0;

	vertexBuffer = en[5]->getMesh()->GetVertexBuffer();
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(en[5]->getMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	context->DrawIndexed(
		en[5]->getMesh()->GetIndexCount(),
		0,
		0);
	//------------------------------------------------------------------------


	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{

	BoundingFrustum camFrustum;
	camera->GetFrustum().Transform(camFrustum, XMMatrixInverse(NULL, XMMatrixTranspose(XMLoadFloat4x4(&camera->GetViewMatrix()))));

	XMFLOAT4X4 proj;
	XMStoreFloat4x4(&proj, XMMatrixTranspose(XMLoadFloat4x4(&camera->GetProjectionMatrix())));

	XMFLOAT3 tempRayDir;
	tempRayDir.x = (((2 * (float)x) / width) - 1) / proj._11;
	tempRayDir.y = -(((2 * (float)y) / height) - 1) / proj._22;
	tempRayDir.z = 1.0f;

	XMMATRIX view = XMMatrixInverse(NULL, XMMatrixTranspose(XMLoadFloat4x4(&camera->GetViewMatrix())));
	XMVECTOR rayDir = XMVector3TransformNormal(XMVector3Normalize(XMLoadFloat3(&tempRayDir)), view);
	XMVECTOR rayPos = XMLoadFloat4(&camera->GetPos()); //Was Load Float 3. If it breaks everything BLAME DARREN
	
	BoundingOrientedBox entBox;
	float distance;

	std::vector<Entity*> entityQueue;
	
	for (size_t i = 0; i < 6; i++)
	{
		en[i]->getMesh()->getOBB().Transform(entBox, XMMatrixTranspose(XMLoadFloat4x4(&en[i]->getWorld())));
		if (camFrustum.Contains(entBox) != ContainmentType::DISJOINT)
		{
			if (entBox.Intersects(rayPos, rayDir, distance))
			{
				entityQueue.push_back(en[i]);
			}
		}
	}

	bool ent_clicked = false;
	Entity* nearestEntity;
	float filterDistance = FLT_MAX;

	for (size_t i = 0; i < entityQueue.size(); ++i)
	{
		Entity* entity = entityQueue[i];

		XMMATRIX entMatrix = XMMatrixInverse(NULL, XMMatrixTranspose(XMLoadFloat4x4(&entity->getWorld())));
		XMVECTOR rayPosLocal = XMVector3TransformCoord(rayPos, entMatrix);
		XMVECTOR rayDirLocal = XMVector3Normalize(XMVector3TransformNormal(rayDir, entMatrix));

		Vertex* verts = entity->getMesh()->GetVertices();
		UINT* faces = entity->getMesh()->GetFaces(); //indices

		for (size_t j = 0; j < entity->getMesh()->GetIndexCount(); j += 3)
		{
			if (TriangleTests::Intersects(rayPosLocal, rayDirLocal, XMLoadFloat3(&verts[faces[j]].Position), XMLoadFloat3(&verts[faces[j + 1]].Position), XMLoadFloat3(&verts[faces[j + 2]].Position), distance))
				if (distance < filterDistance)
				{
					filterDistance = distance;
					nearestEntity = entity;
					ent_clicked = true;
				}
		}
	}
	
	if (ent_clicked)
	{
		printf("\nEntity Mesh name: %s", nearestEntity->getMesh()->GetName().c_str());
		nearestEntity->setPos(XMFLOAT3((nearestEntity->getPos().x+30), nearestEntity->getPos().y, nearestEntity->getPos().z));

	}

	//SceneData::Instance().SelectEntity(nearestEntity);
	

	//Small trigger collider with the mouse x,y to constantly check if it is colliding with the objects


	/*

	//Use directX collision library for BoundingFrustum and BoundingOrientedBox
	
	// Compute picking ray in view space.
	float vx = (+2.0f*x / width - 1.0f) / camera->GetProjectionMatrix()(0,0);
	float vy = (-2.0f*y / height + 1.0f) / camera->GetProjectionMatrix()(1,1);
	// Ray definition in view space.
	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR rayDir = XMVectorSet(vx, vy, 1.0f, 0.0f);

	// Get the inverse of the view matrix.
	XMMATRIX V = XMLoadFloat4x4(&camera->GetViewMatrix());
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);
	
	XMMATRIX W = XMLoadFloat4x4(&en1->getWorld());
	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);

	XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

	rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
	rayDir = XMVector3TransformNormal(rayDir, toLocal);

	// Make the ray direction unit length for the intersection tests.
	rayDir = XMVector3Normalize(rayDir);
	XMFLOAT4 rayDir2;
	XMStoreFloat4(&rayDir2, rayDir);

	printf("\nx:%d y:%d z:%d w:%d",rayDir2.x,rayDir2.y,rayDir2.z,rayDir2.w);
	


	float pointX, pointY;
	XMFLOAT4 direction;
	XMVECTOR rayOrigin, rayDirection;
	bool intersect, result;


	// Move the mouse cursor coordinates into the -1 to +1 range.
	pointX = (((2.0f * (float)x) / (float)width) - 1.0f) / camera->GetProjectionMatrix()(0, 0);
	pointY = ((((2.0f * (float)y) / (float)height) - 1.0f) * -1.0f) / camera->GetProjectionMatrix()(1, 1);

	// Get the inverse of the view matrix.
	XMMATRIX V = XMLoadFloat4x4(&camera->GetViewMatrix());
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);
	XMFLOAT4X4 inverseViewMatrix;
	XMStoreFloat4x4(&inverseViewMatrix,invView);

	// Calculate the direction of the picking ray in view space.
	direction.x = (pointX * inverseViewMatrix._11) + (pointY * inverseViewMatrix._21) + inverseViewMatrix._31;
	direction.y = (pointX * inverseViewMatrix._12) + (pointY * inverseViewMatrix._22) + inverseViewMatrix._32;
	direction.z = (pointX * inverseViewMatrix._13) + (pointY * inverseViewMatrix._23) + inverseViewMatrix._33;
	rayDirection = XMLoadFloat4(&direction);
	// Get the origin of the picking ray which is the position of the camera.
	rayOrigin = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);

	// Get the world matrix and translate to the location of the sphere.
	XMMATRIX W = XMLoadFloat4x4(&en1->getWorld());
	XMMATRIX inverseWorldMatrix = XMMatrixInverse(&XMMatrixDeterminant(W), W);

	XMMATRIX toLocal = XMMatrixMultiply(invView, inverseWorldMatrix);

	rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
	rayDirection = XMVector3TransformNormal(rayDirection, toLocal);
	
	// Normalize the ray direction.
	rayDirection = XMVector3Normalize(rayDirection); 
	XMFLOAT4 rayDir;
	XMStoreFloat4(&rayDir, rayDirection);

	printf("\nx:%d y:%d z:%d w:%d", rayDir.x, rayDir.y, rayDir.z, rayDir.w);
	*/

	//camera->SetMouseDown(true,prevMousePos);
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	camera->SetMouseDown(false,prevMousePos);
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion