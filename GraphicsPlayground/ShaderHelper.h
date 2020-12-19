#pragma once
#include "GraphicsTypes.h"
#include "IShader.h"
#include "ShaderNames.h"
#include "ShaderKey.h"

#include <memory>

namespace ShaderHelper
{
	CompiledShaderObject* CreateCompiledShaderObject(const ShaderName& arFileName);

	std::unique_ptr<IShader> MakeShader(ShaderKey aKey);
};


