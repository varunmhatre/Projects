
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage

Texture2D mat_SRV : register(t0);
SamplerState mat_SS : register(s0);

struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
};

struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

cbuffer ExternalData : register(b0)
{
	DirectionalLight dirLight;
	DirectionalLight dirLight2;
}

float4 main(VertexToPixel input) : SV_TARGET
{ 
	input.normal = normalize(input.normal);
	float3 dirLightDir = normalize(-dirLight.Direction);
	float4 directionalLight = saturate(dot(input.normal, dirLightDir));

	dirLightDir = normalize(-dirLight2.Direction);
	float4 directionalLight2 = saturate(dot(input.normal, dirLightDir));

	float4 surfaceColor = mat_SRV.Sample(mat_SS, input.uv);

	return
		surfaceColor *
		((directionalLight * dirLight.DiffuseColor) +
		(directionalLight2 * dirLight2.DiffuseColor) +
		dirLight.AmbientColor);
}