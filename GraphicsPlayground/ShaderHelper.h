#pragma once
#include "GraphicsTypes.h"
#include "IShader.h"
#include "ShaderNames.h"
#include "ShaderKey.h"

#include <memory>

namespace ShaderHelper
{
	CompiledShaderObject* CreateCompiledShaderObject(const ShaderName& arFileName);

	IShader* MakeShader(ShaderKey aKey);
};


