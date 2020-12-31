#pragma once
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "Texture.h"
#include "ShaderManager.h"
#include <memory>

class App;
class Graphics;

/*
* Holds graphics data used by the game.
*/
class GameGraphics final
{
public:
	GameGraphics() = default;
	~GameGraphics() = default;

	void Init(App& arApp, Graphics& arGfx);
	void Update(App& arApp, Graphics& arGfx);
	void Shutdown(App& arApp, Graphics& arGfx);

private:
	GameGraphics(GameGraphics&) = delete;
	GameGraphics(GameGraphics&&) = delete;
	GameGraphics& operator = (GameGraphics&) = delete;
	GameGraphics& operator = (GameGraphics&&) = delete;

	std::unique_ptr<Model> spTriangle;
	std::unique_ptr<Model> spQuad;
	std::unique_ptr<Model> spCube;
	std::unique_ptr<Model> spSphere;
	std::unique_ptr<PointLight> spLight1;

	std::unique_ptr<Texture> spbraynzar;
	std::unique_ptr<Texture> spGoldAlbedo;
	std::unique_ptr<Texture> spGoldNormal;
	std::unique_ptr<Texture> spGoldRoughness;
	std::unique_ptr<Texture> spGoldMetallic;
	std::unique_ptr<Texture> spBrdf;

	void LoadResources(Graphics& arGfx);

	Camera Cam;
	ShaderManager ShaderMgr;
};

