#pragma once
#include "GraphicsTypes.h"
#include <string>
class Graphics;

class Texture final
{
public:
	Texture() = default;
	~Texture();

	void Init(Graphics& arGraphics, const std::wstring& arName);

	ShaderResourceView* GetTextureView() { return pTextureView; }

private:

	ShaderResourceView* pTextureView = nullptr;
};

