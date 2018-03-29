#include "Material.h"

Material::Material()
{
}

Material::Material(SimplePixelShader * ps, SimpleVertexShader * vs, ID3D11SamplerState* ss, ID3D11ShaderResourceView* alb, ID3D11ShaderResourceView* nm, ID3D11ShaderResourceView* m, ID3D11ShaderResourceView* r, ID3D11ShaderResourceView* ao)
{
	pixShader = ps;
	vertShader = vs;
	mat_SS = ss;
	mat_SRV = alb;
	normal_map = nm;
	metallic = m;
	roughness = r;
	ambient_occlusion = ao;
}


Material::~Material()
{
	if (mat_SRV != NULL) { mat_SRV->Release(); }
	if (normal_map != NULL) { normal_map->Release(); }
	if (metallic != NULL) { metallic->Release(); }
	if (roughness != NULL) { roughness->Release(); }
	if (ambient_occlusion != NULL) { ambient_occlusion->Release(); }
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
	pixShader->SetShaderResourceView("NormalMap", normal_map);
	pixShader->SetShaderResourceView("Metallic", metallic);
	pixShader->SetShaderResourceView("Roughness", roughness);
	pixShader->SetShaderResourceView("AO", ambient_occlusion);
}

void Material::SetSamplerState()
{
	pixShader->SetSamplerState("mat_SS", mat_SS);
}
