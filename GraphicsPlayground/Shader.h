#pragma once
#include "GraphicsInterfaceObject.h"
#include "GraphicsTypes.h"
#include <array>

enum class GlobalDataType
{
	None,
	ProjectionMatrix,
	ViewMatrix,
	WorldMatrix,
};

struct VShaderInfo final
{
	VShaderInfo(VertexShader& arShader, ShaderInputLayout& arLayout, std::array< GlobalDataType, 3>& arCBuffer)
	: spShader(&arShader), spLayout(&arLayout), cBufferTypes(arCBuffer) {}

	GraphicsInterfaceObject<VertexShader> spShader;
	GraphicsInterfaceObject<ShaderInputLayout> spLayout;
	std::array< GlobalDataType, 3> cBufferTypes;
};

struct PShaderInfo final
{
	explicit PShaderInfo(PixelShader& arShader)
		: spShader(&arShader) {}

	GraphicsInterfaceObject<PixelShader> spShader;
};
