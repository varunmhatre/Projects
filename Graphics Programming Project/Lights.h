#pragma once
#include <d3d11.h>
#include "Vertex.h"

using namespace DirectX;

class Lights
{
public:

	struct DirectionalLight
	{
		XMFLOAT4 AmbientColor;
		XMFLOAT4 DiffuseColor;
		XMFLOAT3 Direction;
	};

};

