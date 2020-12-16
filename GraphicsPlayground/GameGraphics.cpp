#include "GameGraphics.h"
#include "Graphics.h"
#include "ShaderNames.h"

namespace
{
	//Define what shaders we want to load here
	const std::vector<ShaderName> g_PShaderNames =
	{
		ShaderNames::BasicPixelShader
	};

	const std::vector<ShaderName> g_VShaderNames =
	{
		ShaderNames::BasicVertShader
	};
}

void GameGraphics::Init(Graphics& arGfx)
{
	auto& rdevice = arGfx.GetDevice();

	arGfx.LoadShaders(g_PShaderNames, g_VShaderNames);

	LoadModels(rdevice);
}

void GameGraphics::LoadModels(GraphicsDevice& arDevice)
{
	spTriangle = Model::CreateTriangle();
	spTriangle->Init(arDevice);
}

void GameGraphics::Update(Graphics& arGfx)
{
	arGfx.AddItemToDraw(spTriangle.get(), ModelInstance());
}