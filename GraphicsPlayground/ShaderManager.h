#pragma once
#include "Shader.h"
#include "ShaderKey.h"
#include <memory>
#include <map>

class ShaderManager final
{
public:
	ShaderManager() = default;
	~ShaderManager() = default;

	void Load(GraphicsDevice& arDevice);

	VShaderInfo& GetVShader(const VShaderKey aKey);
	PShaderInfo& GetPShader(const PShaderKey aKey);

private:
	ShaderManager(ShaderManager&) = delete;
	ShaderManager(ShaderManager&&) = delete;
	ShaderManager& operator = (ShaderManager&) = delete;
	ShaderManager& operator = (ShaderManager&&) = delete;

	std::map<VShaderKey, std::unique_ptr<VShaderInfo>> VsMap;
	std::map<PShaderKey, std::unique_ptr<PShaderInfo>> PsMap;
};

