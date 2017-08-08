#include "Material.h"

Material::Material()
{
}

Material::Material(SimplePixelShader * ps, SimpleVertexShader * vs, ID3D11ShaderResourceView* srv, ID3D11SamplerState* ss)
{
	pixShader = ps;
	vertShader = vs;
	mat_SRV = srv;
	mat_SS = ss;
}


Material::~Material()
{
	mat_SRV->Release();
}

SimplePixelShader * Material::GetPixShader()
{
	return pixShader;
}

SimpleVertexShader * Material::GetVertShader()
{
	return vertShader;
}

ID3D11ShaderResourceView * Material::GetShaderResourceView()
{
	return mat_SRV;
}

ID3D11SamplerState * Material::GetSamplerState()
{
	return mat_SS;
}

void Material::SetShaderResourceView()
{
	pixShader->SetShaderResourceView("mat_SRV", mat_SRV);
}

void Material::SetSamplerState()
{
	pixShader->SetSamplerState("mat_SS", mat_SS);
}
