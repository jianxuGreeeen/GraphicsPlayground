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

	//alignment due to Matrix requiring 16 byte alignment
	void* operator new(size_t i)
	{
		return _aligned_malloc(i, 16);
	}

	void operator delete(void* p)
	{
		_aligned_free(p);
	}
private:
	struct VShaderCBuffer
	{
		Matrix WVP = MatrixS::Identity;
		Matrix World = MatrixS::Identity;;
	}VcBufferData;

	struct PShaderCBuffer
	{
		DirectionalLight DirLight = {
			Color4(.2f, .2f, .2f, 1.0f),
			Float3(1.0f, -1.0f, 0.0f)
		};
		float pad1 = 0.0f;

		PointLight PtLight1;
		float pad2 = 0.0f;
	} PcBufferData;

	PixelShader* pPShader = nullptr;
	VertexShader* pVShader = nullptr;
	ShaderInputLayout* pLayout = nullptr;
	GraphicsBuffer* pVCBuffer = nullptr;
	GraphicsBuffer* pSCBuffer = nullptr;
	ID3D11SamplerState* pSamplerState = nullptr;

	void InitPixelShader(Graphics& arGraphics);
	void InitVertexShader(Graphics& arGraphics);
};

