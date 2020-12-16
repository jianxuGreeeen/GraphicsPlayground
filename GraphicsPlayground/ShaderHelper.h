#pragma once
#include "GraphicsTypes.h"
#include <string>

namespace ShaderHelper
{
	CompiledShaderObject* CreateCompiledShaderObject(const std::wstring& arFileName);

	std::wstring MakeShaderFileName(const std::wstring& arShaderName);
};

