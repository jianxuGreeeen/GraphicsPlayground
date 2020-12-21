#pragma once

#include "GraphicsTypes.h"
#include "Model.h"
#include "GraphicsRasterizerStates.h"
#include "ShaderManager.h"
#include <array>
#include <map>

class App;

struct ModelInstance
{
	//TODO: Add instance data here
	Matrix WorldMatrix;
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

	void LoadResources();
	void AddItemToDraw(Model* const apModel, const ModelInstance& arInstanceData);

	void SetProjectionMatrix(const Matrix& arProjMatrix) { ProjectionMatrix = arProjMatrix; }
	void SetViewMatrix(const Matrix& arViewMatrix) { ViewMatrix = arViewMatrix; }
	void SetRenderState(const RasterizerStates aState) { RasterizerState = aState; }

	// Caller must manage the memory and call release when it's done
	GraphicsBuffer* CreateVertexBuffer(const std::vector<Vertex>& arVerts);
	GraphicsBuffer* CreateIndexBuffer(const std::vector<Index>& arIdices);
	GraphicsBuffer* CreateBuffer(const GraphicsBufferDesc& arDesc, const GraphicsBufferData* apData);

	GraphicsDevice& GetDevice() { return *pDevice; }
	GraphicsDeviceContext& GetDeviceCtx() { return *pDeviceCtx; }
	ShaderManager& GetShaderMgr() { return ShaderMgr; }

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
	RasterizerStates RasterizerState = RasterizerStates::Default;
	ShaderManager ShaderMgr;

	GraphicsViewPort ViewPort;
	Matrix ProjectionMatrix;
	Matrix ViewMatrix;

	std::map<Model* const, std::vector<ModelInstance>> ItemsToDraw;

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
