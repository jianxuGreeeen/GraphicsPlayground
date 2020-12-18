#pragma once
#include "Camera.h"
#include "Model.h"
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

private:
	GameGraphics(GameGraphics&) = delete;
	GameGraphics(GameGraphics&&) = delete;
	GameGraphics& operator = (GameGraphics&) = delete;
	GameGraphics& operator = (GameGraphics&&) = delete;

	std::unique_ptr<Model> spTriangle;
	std::unique_ptr<Model> spQuad;
	std::unique_ptr<Model> spCube;

	void LoadResources(Graphics& arGfx);

	Camera Cam;
};

