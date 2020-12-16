#pragma once
#include "Model.h"
#include <memory>

class Graphics;

/*
* Holds graphics data used by the game.
*/
class GameGraphics final
{
public:
	GameGraphics() = default;
	~GameGraphics() = default;

	void Init(Graphics& arGfx);
	void Update(Graphics& arGfx);

private:
	GameGraphics(GameGraphics&) = delete;
	GameGraphics(GameGraphics&&) = delete;
	GameGraphics& operator = (GameGraphics&) = delete;
	GameGraphics& operator = (GameGraphics&&) = delete;

	std::unique_ptr<Model> spTriangle;

	void LoadModels(GraphicsDevice& arDevice);

};

