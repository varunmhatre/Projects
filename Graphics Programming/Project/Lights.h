#pragma once
#include <d3d11.h>
#include "Vertex.h"

using namespace DirectX;

class Lights
{
public:

	//XMFLOAT4 AmbientColor;

	struct DirectionalLight
	{
		XMFLOAT4 Color;
		XMFLOAT3 Direction;
	};

	struct PointLight
	{
		XMFLOAT4 Color;
		XMFLOAT3 Position;
	};
	
};

