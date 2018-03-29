
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage

Texture2D mat_SRV : register(t0);
Texture2D ShadowMap		: register(t1);
Texture2D NormalMap		: register(t2);
Texture2D Metallic		: register(t3);
Texture2D Roughness		: register(t4);
Texture2D AO		: register(t5);
TextureCube Sky			: register(t6);

SamplerState mat_SS : register(s0);
SamplerComparisonState ShadowSampler : register(s1);

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
	float4 posForShadow : POSITION1;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;
};

struct DirectionalLight
{
	float4 Color;
	float3 Direction;
};

struct PointLight
{
	float4 Color;
	float3 Position;
};

cbuffer ExternalData : register(b0)
{
	/*
	float3  albedo;
	float metallic;
	float roughness;
	float ao;
	*/

	float4 AmbientColor;
	float3 cameraPosition;

	DirectionalLight dirLight;
	DirectionalLight dirLight2;

	PointLight pointLight1;
	PointLight pointLight2;
	PointLight pointLight3;
	PointLight pointLight4;
}

//const float PI = 3.14159265359;

float3 fresnelSchlick(float cosTheta, float3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(float3 N, float3 H, float roughness)
{
	float PI = 3.14159265359;
	float a = roughness*roughness;
	float a2 = a*a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH*NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

float4 main(VertexToPixel input) : SV_TARGET
{ 
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);
	
	// Read and unpack normal from map
	float3 normalFromMap = NormalMap.Sample(mat_SS, input.uv).xyz * 2 - 1;

	// Transform from tangent to world space
	float3 N = input.normal;
	float3 T = normalize(input.tangent - N * dot(input.tangent, N));
	float3 B = cross(T, N);

	float3x3 TBN = float3x3(T, B, N);
	input.normal = normalize(mul(normalFromMap, TBN));
	
	//PBR Stuff
	PointLight PLList[4] = { pointLight1 , pointLight2, pointLight3, pointLight4 };
	float4 FragColor;

	float3 albedo = mat_SRV.Sample(mat_SS, input.uv).rgb;
	float metallic = Metallic.Sample(mat_SS, input.uv).r;
	float roughness = Roughness.Sample(mat_SS, input.uv).r;
	float ao = AO.Sample(mat_SS, input.uv).r;
	
	N = input.normal;
	float3 V = normalize(cameraPosition - input.worldPos);

	float3 F0 = float3(0.04, 0.04, 0.04);
	F0 = lerp(F0, albedo, metallic);
	float PI = 3.14159265359;
	// reflectance equation
	float3 Lo = float3(0.0, 0.0, 0.0);
	for (int i = 0; i < 4; ++i)
	{
		// calculate per-light radiance
		float3 L = normalize(PLList[i].Position - input.worldPos);
		float3 H = normalize(V + L);
		float distance = length(PLList[i].Position - input.worldPos);
		float attenuation = 1.0 / (distance * distance);
		float3 radiance = PLList[i].Color * attenuation;

		// cook-torrance brdf
		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		float3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

		float3 kS = F;
		float3 kD = float3(1.0, 1.0, 1.0) - kS;
		kD *= 1.0 - metallic;

		float3 nominator = NDF * G * F;
		//float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		float denominator = 4.0 * max(max(dot(N, V), 0.0) , max(dot(N, L), 0.0));
		float3 specular = nominator / max(denominator, 0.001);

		//return float4(specular, 1.0);

		// add to outgoing radiance Lo
		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;

	}

	//return float4(Lo, 1.0);
	//return float4(F0, 1.0);

	float3 ambient = float3(0.03, 0.03, 0.03) * albedo *ao;
	float3 color = ambient + Lo;

	//return float4(ambient, 1.0);

	color = color / (color + float3(1.0, 1.0, 1.0));
	color = pow(color, float3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2));

	//return float4(color, 1.0);

	FragColor = float4(color, 1.0);

	return FragColor;

}