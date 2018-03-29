#pragma once

#include "DXCore.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include "Vertex.h"
#include "SimpleShader.h"
#include "WICTextureLoader.h"

using namespace DirectX;

class Material
{
public:
	Material();
	Material(SimplePixelShader*, SimpleVertexShader*, ID3D11SamplerState*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*);
	~Material();

	SimplePixelShader* GetPixShader();
	SimpleVertexShader* GetVertShader();

	ID3D11ShaderResourceView * GetShaderResourceView();
	ID3D11SamplerState * GetSamplerState();

	void SetShaderResourceView();
	void SetSamplerState();

private:

	SimplePixelShader* pixShader;
	SimpleVertexShader* vertShader;

	ID3D11ShaderResourceView* mat_SRV;
	ID3D11ShaderResourceView* normal_map;
	ID3D11ShaderResourceView* metallic;
	ID3D11ShaderResourceView* roughness;
	ID3D11ShaderResourceView* ambient_occlusion;
	ID3D11SamplerState* mat_SS;
};

