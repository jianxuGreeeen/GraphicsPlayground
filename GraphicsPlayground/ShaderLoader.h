#pragma once
#include "Shader.h"
#include <functional>
#include <memory>

// Add the loading function here for the shader
using VShaderLoadFunc = std::function<std::unique_ptr<VShaderInfo>(GraphicsDevice&)>;
namespace VShaderLoader
{
	std::unique_ptr<VShaderInfo> LoadBasicShader(GraphicsDevice& arDevice);
}

using PShaderLoadFunc = std::function<std::unique_ptr<PShaderInfo>(GraphicsDevice&)>;
namespace PShaderLoader
{
	std::unique_ptr<PShaderInfo> LoadBasicShader(GraphicsDevice& arDevice);
}

