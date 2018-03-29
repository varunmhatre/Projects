#pragma once

#include "DXCore.h"
#include "SimpleShader.h"
#include <DirectXMath.h>
#include "Entity.h"
#include "Camera.h"
#include "Material.h"
#include "Lights.h"
#include <vector>

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown (WPARAM buttonState, int x, int y);
	void OnMouseUp	 (WPARAM buttonState, int x, int y);
	void OnMouseMove (WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta,   int x, int y);
private:

	// Initialization helper methods - feel free to customize, combine, etc.
	Camera* camera;
	void LoadShaders(); 
	//void CreateMatrices();
	void CreateBasicGeometry();

	// Buffers to hold actual geometry data
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// The matrices to go from model space to screen space
	//DirectX::XMFLOAT4X4 worldMatrix;
	//DirectX::XMFLOAT4X4 viewMatrix;
	//DirectX::XMFLOAT4X4 projectionMatrix;

	// Keeps track of the old mouse position.  Useful for 
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;

	Mesh* figure1;
	Mesh* figure2;
	Mesh* figure3;

	Mesh* cone;
	Mesh* helix;
	Mesh* cube;
	Mesh* cylinder;
	Mesh* sphere;
	Mesh* torus;


	//std::vector<Entity*> en;
	//std::vector<Mesh*> en;
	//std::vector<Material*> en;
	Entity* en[6];
	Entity* BG;

	Material* m1;
	Material* m2;
	Material* m3;
	Material* m4;

	ID3D11ShaderResourceView* crateSRV;

	ID3D11ShaderResourceView* leatherSRV;
	ID3D11ShaderResourceView* leatherNormalSRV;
	ID3D11ShaderResourceView* leatherMetalSRV;
	ID3D11ShaderResourceView* leatherRoughnessSRV;
	ID3D11ShaderResourceView* leatherAOSRV;

	ID3D11ShaderResourceView* plateFloorSRV;
	ID3D11ShaderResourceView* plateFloorNormalSRV;
	ID3D11ShaderResourceView* plateFloorMetalSRV;
	ID3D11ShaderResourceView* plateFloorRoughnessSRV;
	ID3D11ShaderResourceView* plateFloorAOSRV;

	ID3D11ShaderResourceView* metalSRV;
	ID3D11ShaderResourceView* metalNormalSRV;
	ID3D11ShaderResourceView* metalMetalSRV;
	ID3D11ShaderResourceView* metalRoughnessSRV;
	ID3D11ShaderResourceView* metalAOSRV;

	ID3D11ShaderResourceView* metal2SRV;
	ID3D11ShaderResourceView* metal2NormalSRV;
	ID3D11ShaderResourceView* metal2MetalSRV;
	ID3D11ShaderResourceView* metal2RoughnessSRV;
	ID3D11ShaderResourceView* metal2AOSRV;

	ID3D11SamplerState* sampState;

	void RenderShadowMap();

	// Shadow stuff
	int shadowMapSize;
	ID3D11DepthStencilView* shadowDSV;
	ID3D11ShaderResourceView* shadowSRV;
	ID3D11SamplerState* shadowSampler;
	ID3D11RasterizerState* shadowRasterizer;
	SimpleVertexShader* shadowVS;
	DirectX::XMFLOAT4X4 shadowViewMatrix;
	DirectX::XMFLOAT4X4 shadowProjectionMatrix;

	// Sky stuff
	ID3D11ShaderResourceView* skySRV;
	SimpleVertexShader* skyVS;
	SimplePixelShader* skyPS;
	ID3D11RasterizerState* rsSky;
	ID3D11DepthStencilState* dsSky;

};

