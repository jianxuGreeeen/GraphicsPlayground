#include "ShaderLayouts.h"
#pragma warning(disable : 26812)

namespace ShaderLayouts
{
	std::vector<ShaderLayout> GetBasicLayout()
	{
		return 
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
	}
};