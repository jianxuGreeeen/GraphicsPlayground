#pragma once
#include "IShader.h"
#include "ShaderKey.h"
#include <memory>
#include <map>

class Graphics;

class ShaderManager final
{
public:
	ShaderManager() = default;
	~ShaderManager() = default;

	void Load(Graphics& arGraphics);
	void Shutdown(Graphics& arGraphics);

	IShader* GetShader(const ShaderKey aKey);

private:
	ShaderManager(ShaderManager&) = delete;
	ShaderManager(ShaderManager&&) = delete;
	ShaderManager& operator = (ShaderManager&) = delete;
	ShaderManager& operator = (ShaderManager&&) = delete;

	std::map<ShaderKey, std::unique_ptr<IShader>> Map;
};

