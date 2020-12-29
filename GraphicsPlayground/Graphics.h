#pragma once

#include "GraphicsTypes.h"
#include "Light.h"
#include "Model.h"
#include "GraphicsRasterizerStates.h"
#include <array>
#include <map>

class App;
class Texture;
class IShader;

struct ModelInstance
{
	//Add instance data here
	Matrix WorldMatrix;
	Texture* pTexture = nullptr;
};

struct GraphicsDrawState
{
	Model* pModel = nullptr;
	IShader* pShader = nullptr;
	RasterizerStates RasterizerState = RasterizerStates::Default;

	bool operator < (const GraphicsDrawState& arOther) const
	{
		return pShader < arOther.pShader ||
			pModel < arOther.pModel ||
			static_cast<int>(RasterizerState) < static_cast<int>(arOther.RasterizerState);
	}
};

/*
* Handles the graphics pipeline
*/
class Graphics final
{
public:
	Graphics() = default;
	~Graphics() = default;

	void Init(const App& arApp);
	void Update();
	void Draw();
	void Shutdown();

	void AddItemToDraw(const GraphicsDrawState& arDrawState, const ModelInstance& arInstanceData);
	void AddPointLight(const PointLight& arInstance);

	void SetProjectionMatrix(const Matrix& arProjMatrix) { ProjectionMatrix = arProjMatrix; }
	void SetViewMatrix(const Matrix& arViewMatrix) { ViewMatrix = arViewMatrix; }
	
	// Caller must manage the memory and call release when it's done
	GraphicsBuffer* CreateVertexBuffer(const std::vector<Vertex>& arVerts);
	GraphicsBuffer* CreateIndexBuffer(const std::vector<Index>& arIdices);
	GraphicsBuffer* CreateBuffer(const GraphicsBufferDesc& arDesc, const GraphicsBufferData* apData);

	GraphicsDevice& GetDevice() { return *pDevice; }
	GraphicsDeviceContext& GetDeviceCtx() { return *pDeviceCtx; }

private:
	Graphics(Graphics&) = delete;
	Graphics(Graphics&&) = delete;
	Graphics& operator = (Graphics&) = delete;
	Graphics& operator = (Graphics&&) = delete;

	GraphicsFactory* pFactory;
	GraphicsAdapter* pAdapter;
	GraphicsDevice* pDevice;
	GraphicsDeviceContext* pDeviceCtx;
	SwapChain* pSwapChain;
	RenderTarget* pRenderTarget;
	GraphicsDepthStencil* pDepthStencil;
	GraphicsDepthStencilBuffer* pDepthStencilBuffer;
	std::array< GraphicsRasterizerState*, static_cast<size_t>(RasterizerStates::Count)> pRasterizerStates;

	D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	GraphicsViewPort ViewPort;
	Matrix ProjectionMatrix;
	Matrix ViewMatrix;

	std::map<GraphicsDrawState, std::vector<ModelInstance>> ItemsToDraw;
	PointLight PointLight1;

	void InitAdapter();
	void InitFactory();
	void InitDevice();
	void InitSwapChain(const App& arApp);
	void InitRenderTargets();
	void InitDepthStencil(const App& arApp);
	void InitRasterizerStates();

	void BeginFrame();
	void EndFrame();
	void PresentFrame();

	void ValidateDevice();
};
