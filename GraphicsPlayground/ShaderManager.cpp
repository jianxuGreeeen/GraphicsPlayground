#include "ShaderManager.h"
#include "ShaderHelper.h"
#include <assert.h>
#include <utility>

void ShaderManager::Load(Graphics& arGraphics)
{
	if (Map.empty())
	{
		Map.emplace(ShaderKey::BasicShader, nullptr);
	}
	
	for (auto& kvp : Map)
	{
		auto spshader = ShaderHelper::MakeShader(kvp.first);
		spshader->Init(arGraphics);
		kvp.second = std::move(spshader);
	}
}

void ShaderManager::Shutdown(Graphics& arGraphics)
{
	for (auto& kvp : Map)
	{
		auto* pshader = kvp.second.get();
		pshader->Release();
	}
}

IShader* ShaderManager::GetShader(const ShaderKey aKey)
{
	assert(Map.find(aKey) != Map.end());
	return Map[aKey].get();
}

