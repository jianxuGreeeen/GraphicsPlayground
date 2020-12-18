#pragma once
#include "GraphicsInterfaceObject.h"
#include "GraphicsTypes.h"
#include <array>

struct VShaderInfo final
{
	VShaderInfo(VertexShader& arShader, ShaderInputLayout& arLayout)
	: spShader(&arShader), spLayout(&arLayout) {}

	GraphicsInterfaceObject<VertexShader> spShader;
	GraphicsInterfaceObject<ShaderInputLayout> spLayout;
};

struct PShaderInfo final
{
	explicit PShaderInfo(PixelShader& arShader)
		: spShader(&arShader) {}

	GraphicsInterfaceObject<PixelShader> spShader;
};
