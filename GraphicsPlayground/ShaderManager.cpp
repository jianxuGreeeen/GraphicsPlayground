#include "ShaderManager.h"
#include "ShaderHelper.h"
#include <assert.h>
#include <utility>

void ShaderManager::Load(Graphics& arGraphics)
{
	if (Map.empty())
	{
		Map.emplace(ShaderKey::BasicShader, nullptr);
		Map.emplace(ShaderKey::PbrShader, nullptr);
	}
	
	for (auto& kvp : Map)
	{
		auto spshader = ShaderHelper::MakeShader(kvp.first);
		spshader->Init(arGraphics);
		kvp.second.reset(spshader);
	}
}

void ShaderManager::Shutdown(Graphics& arGraphics)
{
	for (auto& kvp : Map)
	{
		auto* pshader = kvp.second.get();
		pshader->Release();
		kvp.second.release();
	}
}

IShader* ShaderManager::GetShader(const ShaderKey aKey)
{
	assert(Map.find(aKey) != Map.end());
	return Map[aKey].get();
}

