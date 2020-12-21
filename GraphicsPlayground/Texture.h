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
	ID3D11SamplerState* GetSamplerState() { return pSamplerState; }

private:

	ShaderResourceView* pTextureView = nullptr;
	ID3D11SamplerState* pSamplerState = nullptr;
};

