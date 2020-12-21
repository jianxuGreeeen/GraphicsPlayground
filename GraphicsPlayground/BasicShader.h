#pragma once
#include "IShader.h"
#include "GraphicsTypes.h"
#include <memory>

class BasicShader final : public IShader
{
public:
	virtual void Init(Graphics& arGraphics) override;
	virtual void Update(Graphics& arGraphics) override;
	virtual void UpdateCBuffers(Graphics& arGraphics, const Matrix& arWVPMatrix, Texture* apTexture) override;
	virtual void Release() override;

private:
	 PixelShader* pPShader = nullptr;
	 VertexShader* pVShader = nullptr;
	 ShaderInputLayout* pLayout = nullptr;
	 GraphicsBuffer* pVCBuffer = nullptr;

	void InitPixelShader(Graphics& arGraphics);
	void InitVertexShader(Graphics& arGraphics);
};

