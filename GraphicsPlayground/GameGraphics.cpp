#include "GameGraphics.h"
#include "Graphics.h"
#include "ShaderKey.h"

void GameGraphics::LoadResources(Graphics& arGfx)
{
	auto& rdevice = arGfx.GetDevice();
	spTriangle = Model::CreateTriangle();
	spTriangle->Init(rdevice);
}

void GameGraphics::Update(Graphics& arGfx)
{
	arGfx.AddItemToDraw(spTriangle.get(), ModelInstance());
}