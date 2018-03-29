#include "Game.h"
#include "Vertex.h"
#include <DirectXCollision.h>
#include "WICTextureLoader.h" // From DirectX Tool Kit
#include "DDSTextureLoader.h" // For loading skyboxes (cube maps)

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
	delete BG;

	delete m1;
	delete m2;
	delete m3;

	// Clean up shadow map
	shadowDSV->Release();
	shadowSRV->Release();
	shadowRasterizer->Release();
	shadowSampler->Release();
	delete shadowVS;

	// Clean up cube map
	skySRV->Release();
	rsSky->Release();
	dsSky->Release();
	delete skyVS;
	delete skyPS;

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

	//CreateWICTextureFromFile(device, context, L"Debug/Assets/Textures/crate.png", 0, &crateSRV);
	CreateWICTextureFromFile(device, context, L"Debug/Assets/Textures/albedo/TuftedLeather.tiff", 0, &leatherSRV);
	CreateWICTextureFromFile(device, context, L"Debug/Assets/Textures/normal/TuftedLeather.tiff", 0, &leatherNormalSRV);
	//CreateWICTextureFromFile(device, context, L"Debug/Assets/Textures/metallic/TuftedLeather.tiff", 0, &leatherMetalSRV);
	CreateWICTextureFromFile(device, context, L"Debug/Assets/Textures/roughness/TuftedLeather.tiff", 0, &leatherRoughnessSRV);
	CreateWICTextureFromFile(device, context, L"Debug/Assets/Textures/ao/TuftedLeather.tiff", 0, &leatherAOSRV);

	CreateWICTextureFromFile(device, context, L"Debug/Assets/Textures/albedo/BareThreadplateFloor.tiff", 0, &plateFloorSRV);
	CreateWICTextureFromFile(device, context, L"Debug/Assets/Textures/normal/BareThreadplateFloor.tiff", 0, &plateFloorNormalSRV);
	CreateWICTextureFromFile(device, context, L"Debug/Assets/Textures/metallic/BareThreadplateFloor.tiff", 0, &plateFloorMetalSRV);
	CreateWICTextureFromFile(device, context, L"Debug/Assets/Textures/roughness/BareThreadplateFloor.tiff", 0, &plateFloorRoughnessSRV);
	CreateWICTextureFromFile(device, context, L"Debug/Assets/Textures/ao/BareThreadplateFloor.tiff", 0, &plateFloorAOSRV);

	CreateWICTextureFromFile(device, context, L"Debug/Assets/Textures/albedo/StainlessGunMetal.tiff", 0, &metalSRV);
	CreateWICTextureFromFile(device, context, L"Debug/Assets/Textures/normal/StainlessGunMetal.tiff", 0, &metalNormalSRV);
	CreateWICTextureFromFile(device, context, L"Debug/Assets/Textures/metallic/StainlessGunMetal.tiff", 0, &metalMetalSRV);
	CreateWICTextureFromFile(device, context, L"Debug/Assets/Textures/roughness/StainlessGunMetal.tiff", 0, &metalRoughnessSRV);
	//CreateWICTextureFromFile(device, context, L"Debug/Assets/Textures/ao/StainlessGunMetal.tiff", 0, &metalAOSRV);

	CreateWICTextureFromFile(device, context, L"Debug/Assets/Textures/albedo/DeepDamageMetal.tiff", 0, &metal2SRV);
	CreateWICTextureFromFile(device, context, L"Debug/Assets/Textures/normal/DeepDamageMetal.tiff", 0, &metal2NormalSRV);
	CreateWICTextureFromFile(device, context, L"Debug/Assets/Textures/metallic/DeepDamageMetal.tiff", 0, &metal2MetalSRV);
	CreateWICTextureFromFile(device, context, L"Debug/Assets/Textures/roughness/DeepDamageMetal.tiff", 0, &metal2RoughnessSRV);
	CreateWICTextureFromFile(device, context, L"Debug/Assets/Textures/ao/DeepDamageMetal.tiff", 0, &metal2AOSRV);
	

	CreateDDSTextureFromFile(device, L"Debug/Assets/Textures/SunnyCubeMap.dds", 0, &skySRV);

	D3D11_SAMPLER_DESC samDesc = {};

	samDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samDesc, &sampState);

	// Set up sky stuff

	// Set up the rasterize state
	D3D11_RASTERIZER_DESC rsDesc = {};
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_FRONT;
	rsDesc.DepthClipEnable = true;
	device->CreateRasterizerState(&rsDesc, &rsSky);

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	device->CreateDepthStencilState(&dsDesc, &dsSky);

	m1 = new Material(pixelShader, vertexShader, sampState, leatherSRV, leatherNormalSRV, NULL, leatherRoughnessSRV, leatherAOSRV);
	m2 = new Material(pixelShader, vertexShader, sampState, plateFloorSRV, plateFloorNormalSRV, plateFloorMetalSRV, plateFloorRoughnessSRV, plateFloorAOSRV);
	m3 = new Material(pixelShader, vertexShader, sampState, metalSRV, metalNormalSRV, metalMetalSRV, metalRoughnessSRV, NULL);
	m4 = new Material(pixelShader, vertexShader, sampState, metal2SRV, metal2NormalSRV, metal2MetalSRV, metal2RoughnessSRV, metal2AOSRV);

	CreateBasicGeometry();

	camera = new Camera(width, height);


	// Shadow view matrix (where the light is looking from)
	XMMATRIX shView = XMMatrixLookAtLH(
		XMVectorSet(0, 20, -20, 0), // Eye position
		XMVectorSet(0, 0, 0, 0),		// Look at pos
		XMVectorSet(0, 1, 0, 0));		// Up
	XMStoreFloat4x4(&shadowViewMatrix, XMMatrixTranspose(shView));

	// Shadow proj matrix
	XMMATRIX shProj = XMMatrixOrthographicLH(
		10.0f,		// Ortho width
		10.0f,		// Ortho height
		0.1f,		// Near plane
		100.0f);	// Far plane
	XMStoreFloat4x4(&shadowProjectionMatrix, XMMatrixTranspose(shProj));

	Lights::DirectionalLight dirL;
	//Lights::AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1);

	//dirL.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1);
	dirL.Color = XMFLOAT4(0, 0, 0, 1);
	dirL.Direction = XMFLOAT3(1, 1, 0);

	pixelShader->SetData(
		"dirLight",  // The name of the (eventual) variable in the shader
		&dirL,   // The address of the data to copy
		sizeof(Lights::DirectionalLight)); // The size of the data to copy

	Lights::DirectionalLight dirLDos;

	//dirLDos.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1);
	dirLDos.Color = XMFLOAT4(0, 0, 0, 1);
	dirLDos.Direction = XMFLOAT3(1, 0, 0);

	pixelShader->SetData("dirLight2", &dirLDos, sizeof(Lights::DirectionalLight));

	Lights::PointLight pointL1;
	pointL1.Color = XMFLOAT4(300, 300, 300, 1);
	pointL1.Position = XMFLOAT3(-10, -10, -10);
	pixelShader->SetData("pointLight1", &pointL1, sizeof(Lights::PointLight));

	Lights::PointLight pointL2;
	pointL2.Color = XMFLOAT4(300, 300, 300, 1);
	pointL2.Position = XMFLOAT3(10, -10, -10);
	pixelShader->SetData("pointLight2", &pointL2, sizeof(Lights::PointLight));

	Lights::PointLight pointL3;
	pointL3.Color = XMFLOAT4(300, 300, 300, 1);
	pointL3.Position = XMFLOAT3(-10, 10, -10);
	pixelShader->SetData("pointLight3", &pointL3, sizeof(Lights::PointLight));
	//pixelShader->SetData("AmbientColor", &Lights::AmbientColor, sizeof(XMFLOAT4));
	pixelShader->SetFloat4("AmbientColor", XMFLOAT4(0.1f, 0.1f, 0.1f, 1));

	Lights::PointLight pointL4;
	pointL4.Color = XMFLOAT4(300, 300, 300, 1);
	pointL4.Position = XMFLOAT3(10, 10, -10);
	pixelShader->SetData("pointLight4", &pointL4, sizeof(Lights::PointLight));

	pixelShader->SetFloat3("cameraPosition", XMFLOAT3(camera->GetPos().x, camera->GetPos().y, camera->GetPos().z));

	// Create shadow requirements ------------------------------------------
	shadowMapSize = 2048;


	// Create the actual texture that will be the shadow map
	D3D11_TEXTURE2D_DESC shadowDesc = {};
	shadowDesc.Width = shadowMapSize;
	shadowDesc.Height = shadowMapSize;
	shadowDesc.ArraySize = 1;
	shadowDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	shadowDesc.CPUAccessFlags = 0;
	shadowDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	shadowDesc.MipLevels = 1;
	shadowDesc.MiscFlags = 0;
	shadowDesc.SampleDesc.Count = 1;
	shadowDesc.SampleDesc.Quality = 0;
	shadowDesc.Usage = D3D11_USAGE_DEFAULT;
	ID3D11Texture2D* shadowTexture;
	device->CreateTexture2D(&shadowDesc, 0, &shadowTexture);

	// Create the depth/stencil
	D3D11_DEPTH_STENCIL_VIEW_DESC shadowDSDesc = {};
	shadowDSDesc.Format = DXGI_FORMAT_D32_FLOAT;
	shadowDSDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	shadowDSDesc.Texture2D.MipSlice = 0;
	device->CreateDepthStencilView(shadowTexture, &shadowDSDesc, &shadowDSV);

	// Create the SRV for the shadow map
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	device->CreateShaderResourceView(shadowTexture, &srvDesc, &shadowSRV);

	// Release the texture reference since we don't need it
	shadowTexture->Release();

	// Create the special "comparison" sampler state for shadows
	D3D11_SAMPLER_DESC shadowSampDesc = {};
	shadowSampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR; // Could be anisotropic
	shadowSampDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	shadowSampDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	shadowSampDesc.BorderColor[0] = 1.0f;
	shadowSampDesc.BorderColor[1] = 1.0f;
	shadowSampDesc.BorderColor[2] = 1.0f;
	shadowSampDesc.BorderColor[3] = 1.0f;
	device->CreateSamplerState(&shadowSampDesc, &shadowSampler);

	// Create a rasterizer state
	D3D11_RASTERIZER_DESC shadowRastDesc = {};
	shadowRastDesc.FillMode = D3D11_FILL_SOLID;
	shadowRastDesc.CullMode = D3D11_CULL_BACK;
	shadowRastDesc.DepthClipEnable = true;
	shadowRastDesc.DepthBias = 1000; // Multiplied by (smallest possible value > 0 in depth buffer)
	shadowRastDesc.DepthBiasClamp = 0.0f;
	shadowRastDesc.SlopeScaledDepthBias = 1.0f;
	device->CreateRasterizerState(&shadowRastDesc, &shadowRasterizer);

	//End Shadow stuff-----------------------------------------

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

	shadowVS = new SimpleVertexShader(device, context);
	if (!shadowVS->LoadShaderFile(L"Debug/ShadowVS.cso"))
		shadowVS->LoadShaderFile(L"ShadowVS.cso");

	skyVS = new SimpleVertexShader(device, context);
	if (!skyVS->LoadShaderFile(L"Debug/SkyVS.cso"))
		skyVS->LoadShaderFile(L"SkyVS.cso");

	skyPS = new SimplePixelShader(device, context);
	if (!skyPS->LoadShaderFile(L"Debug/SkyPS.cso"))
		skyPS->LoadShaderFile(L"SkyPS.cso");

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

	en[0] = new Entity(sphere, m1, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));
	en[1] = new Entity(sphere, m1, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));
	en[2] = new Entity(cube, m1, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));
	en[3] = new Entity(sphere, m1, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));
	en[4] = new Entity(sphere, m2, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));
	en[5] = new Entity(sphere, m3, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));

	BG = new Entity(cube, m1, XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(10, 10, 0.5));

	
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
	en[2]->setPos(XMFLOAT3(1, -1, 0));
	en[1]->setPos(XMFLOAT3(-0.3, -0.3, -3.5));
	en[3]->setPos(XMFLOAT3(-0.3, 0.3, -3.5));
	en[4]->setPos(XMFLOAT3(0.3, 0.3, -3.5));
	en[5]->setPos(XMFLOAT3(0.3, -0.3, -3.5));

	BG->setPos(XMFLOAT3(0, 0, 2));
}

void Game::RenderShadowMap()
{
	// Initial setup: No RTV (remember to clear shadow map)
	context->OMSetRenderTargets(0, 0, shadowDSV);
	context->ClearDepthStencilView(shadowDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	context->RSSetState(shadowRasterizer);

	// Set up an appropriate shadow view port
	D3D11_VIEWPORT shadowVP = {};
	shadowVP.TopLeftX = 0;
	shadowVP.TopLeftY = 0;
	shadowVP.Width = (float)shadowMapSize;
	shadowVP.Height = (float)shadowMapSize;
	shadowVP.MinDepth = 0.0f;
	shadowVP.MaxDepth = 1.0f;
	context->RSSetViewports(1, &shadowVP);

	// Set up shaders for making the shadow map
	shadowVS->SetShader();
	shadowVS->SetMatrix4x4("view", shadowViewMatrix);
	shadowVS->SetMatrix4x4("projection", shadowProjectionMatrix);

	// Turn off pixel shader
	context->PSSetShader(0, 0, 0);

	// Actually draw the entities
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	for (unsigned int i = 0; i < 6; i++)
	{

		// Grab the data from the first entity's mesh
		Entity* ge = en[i];
		ID3D11Buffer* vb = ge->getMesh()->GetVertexBuffer();
		ID3D11Buffer* ib = ge->getMesh()->GetIndexBuffer();

		// Set buffers in the input assembler
		context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

		shadowVS->SetMatrix4x4("world", ge->getWorld());
		shadowVS->CopyAllBufferData();

		// Finally do the actual drawing
		context->DrawIndexed(ge->getMesh()->GetIndexCount(), 0, 0);
	}

	// Revert to original targets and states
	context->OMSetRenderTargets(1, &this->backBufferRTV, this->depthStencilView);
	shadowVP.Width = (float)this->width;
	shadowVP.Height = (float)this->height;
	context->RSSetViewports(1, &shadowVP);
	context->RSSetState(0);
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

	en[0]->setRot(XMFLOAT3(0, totalTime / 2, 0));
	en[1]->setRot(XMFLOAT3(0, totalTime / 2, 0));
	en[2]->setRot(XMFLOAT3(0, totalTime / 2, 0));
	en[3]->setRot(XMFLOAT3(0, totalTime / 2, 0));
	en[4]->setRot(XMFLOAT3(0, totalTime / 2, 0));
	en[5]->setRot(XMFLOAT3(0, totalTime / 2, 0));
	
	camera->Update(prevMousePos, deltaTime);

}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{

	// Take care of shadow rendering
	RenderShadowMap();

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

	
	BG->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix(), shadowViewMatrix, shadowProjectionMatrix, shadowSampler, shadowSRV);

	stride = sizeof(Vertex);
	offset = 0;

	vertexBuffer = BG->getMesh()->GetVertexBuffer();
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(BG->getMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	//context->DrawIndexed(
	//	BG->getMesh()->GetIndexCount(),
	//	0,
	//	0);
	
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------

	en[0]->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix(), shadowViewMatrix, shadowProjectionMatrix, shadowSampler, shadowSRV);

	stride = sizeof(Vertex);
	offset = 0;

	vertexBuffer = en[0]->getMesh()->GetVertexBuffer();
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(en[0]->getMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	//context->DrawIndexed(
	//	en[0]->getMesh()->GetIndexCount(),
	//	0,     
	//	0);    
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------

	en[1]->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix(), shadowViewMatrix, shadowProjectionMatrix, shadowSampler, shadowSRV);

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

	en[2]->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix(), shadowViewMatrix, shadowProjectionMatrix, shadowSampler, shadowSRV);

	stride = sizeof(Vertex);
	offset = 0;

	vertexBuffer = en[2]->getMesh()->GetVertexBuffer();
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(en[2]->getMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	//context->DrawIndexed(
	//	en[2]->getMesh()->GetIndexCount(),
	//	0,
	//	0);
	//------------------------------------------------------------------------

	//------------------------------------------------------------------------

	//Set m2 as material for next objects
	m4->SetShaderResourceView();
	m4->SetSamplerState();

	en[3]->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix(), shadowViewMatrix, shadowProjectionMatrix, shadowSampler, shadowSRV);

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

	//Set m2 as material for next objects
	m2->SetShaderResourceView();
	m2->SetSamplerState();

	en[4]->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix(), shadowViewMatrix, shadowProjectionMatrix, shadowSampler, shadowSRV);

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

	//Set m3 as material for next objects
	m3->SetShaderResourceView();
	m3->SetSamplerState();

	en[5]->PrepareMaterial(camera->GetViewMatrix(), camera->GetProjectionMatrix(), shadowViewMatrix, shadowProjectionMatrix, shadowSampler, shadowSRV);

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

	// Draw the sky ------------------------
	vertexBuffer = en[2]->getMesh()->GetVertexBuffer();
	
	// Set buffers in the input assembler
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(en[2]->getMesh()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	// Set up the sky shaders
	skyVS->SetMatrix4x4("view", camera->GetViewMatrix());
	skyVS->SetMatrix4x4("projection", camera->GetProjectionMatrix());
	skyVS->CopyAllBufferData();
	skyVS->SetShader();

	skyPS->SetShaderResourceView("Sky", skySRV);
	skyPS->CopyAllBufferData();
	skyPS->SetShader();

	context->RSSetState(rsSky);
	context->OMSetDepthStencilState(dsSky, 0);
	context->DrawIndexed(en[2]->getMesh()->GetIndexCount(), 0, 0);

	// Reset the render states we've changed
	context->RSSetState(0);
	context->OMSetDepthStencilState(0, 0);

	// Unbind the shadow map from PS input
	pixelShader->SetShaderResourceView("ShadowMap", 0);

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
	camera->SetMouseDown(true, prevMousePos);

	//Working Picking Algorithm
	/*
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
	*/

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