#pragma once
#include "IShader.h"
#include "GraphicsTypes.h"
#include "Light.h"
#include <memory>

class PbrShader final : public IShader
{
public:
	virtual void Init(Graphics& arGraphics) override;
	virtual void Update(Graphics& arGraphics) override;
	virtual void UpdateCBuffers(Graphics& arGraphics, const ShaderBufferConstants arKey, const void* apData) override;
	virtual void CommitCBufferData(Graphics& arGraphics) override;
	virtual void UpdateTexture(Graphics& arGraphics, const TextureKey arKey, Texture* const apData) override;
	virtual void Release() override;
};

