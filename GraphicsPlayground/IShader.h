#pragma once
#include "MathTypes.h"
#include "ShaderBufferConstants.h"
#include "TextureKey.h"

class Graphics;
class Texture;

/*
* To make a new shader:
* 1) Create a class that inherits from IShader
* 2) Add a corresponding key enum to ShaderKey.h
* 3) Add a corresponding case to the MakeShader function in ShaderHelper.h
*/

class IShader
{
public:
	virtual void Init(Graphics& arGraphics) = 0;
	virtual void Update(Graphics& arGraphics) = 0;
	virtual void UpdateCBuffers(Graphics& arGraphics, const ShaderBufferConstants arKey, const void* apData) = 0;
	virtual void CommitCBufferData(Graphics& arGraphics) = 0;
	virtual void UpdateTexture(Graphics& arGraphics, const TextureKey arKey, Texture* const apData) = 0;
	virtual void Release() = 0;
};
