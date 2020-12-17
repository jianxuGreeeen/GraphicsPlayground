#include "ShaderManager.h"
#include "ShaderLoader.h"
#include "ShaderKey.h"

namespace
{
	struct VShaderLoadInfo
	{
		VShaderKey Key;
		VShaderLoadFunc Loader;
	};
	struct PShaderLoadInfo
	{
		PShaderKey Key;
		PShaderLoadFunc Loader;
	};

	// Vertex shaders to load
	VShaderLoadInfo VLoadData[] =
	{
		{VShaderKey::BasicShader, VShaderLoader::LoadBasicShader}
	};

	// Pixel shaders to load
	PShaderLoadInfo PLoadData[] =
	{
		{PShaderKey::BasicShader, PShaderLoader::LoadBasicShader}
	};
}

void ShaderManager::Load(GraphicsDevice& arDevice)
{
	for (const auto& rinfo : VLoadData)
	{
		auto spshader = rinfo.Loader(arDevice);
		VsMap.emplace(rinfo.Key, std::move(spshader));
	}

	for (const auto& rinfo : PLoadData)
	{
		auto spshader = rinfo.Loader(arDevice);
		PsMap.emplace(rinfo.Key, std::move(spshader));
	}
}

VShaderInfo& ShaderManager::GetVShader(const VShaderKey aKey)
{
	return *(VsMap[aKey]);
}

PShaderInfo& ShaderManager::GetPShader(const PShaderKey aKey)
{
	return *(PsMap[aKey]);
}

