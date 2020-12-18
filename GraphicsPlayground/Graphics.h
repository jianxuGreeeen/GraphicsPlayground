#pragma once

#include "Camera.h"
#include "GraphicsInterfaceObject.h"
#include "GraphicsTypes.h"
#include "Model.h"
#include "ShaderManager.h"

#include <array>
#include <d3d11.h>
#include <dxgi1_4.h>
#include <map>

struct ModelInstance
{
	//TODO: Add instance data here
	Matrix WorldMatrix;
};

class App;

/*
* Handles the graphics pipeline
*/
class Graphics final
{
public:
	Graphics() = default;
	~Graphics() = default;

	void Init();
	void PrepForWindow(const App& arApp);
	void Update();
	void Draw();
	void Shutdown();

	void LoadResources();
	void AddItemToDraw(Model* const apModel, const ModelInstance& arInstanceData);

	void SetProjectionMatrix(const Matrix& arProjMatrix) { ProjectionMatrix = arProjMatrix; }
	void SetViewMatrix(const Matrix& arViewMatrix) { ViewMatrix = arViewMatrix; }

	GraphicsDevice& GetDevice() { return *spDevice; }
	ShaderManager& GetShaderMgr() { return ShaderMgr; }

private:
	Graphics(Graphics&) = delete;
	Graphics(Graphics&&) = delete;
	Graphics& operator = (Graphics&) = delete;
	Graphics& operator = (Graphics&&) = delete;

	template<class T>
	using GIO = GraphicsInterfaceObject<T>;
	GIO<GraphicsFactory> spFactory;
	GIO<GraphicsAdapter> spAdapter;
	GIO<GraphicsDevice> spDevice;
	GIO<GraphicsDeviceContext> spDeviceCtx;
	GIO<SwapChain> spSwapChain;
	GIO<RenderTarget> spRenderTarget;
	GIO<GraphicsDepthStencil> spDepthStencil;
	GIO<GraphicsDepthStencilBuffer> spDepthStencilBuffer;
	GIO<GraphicsRasterizerState> spRasterizerState;
	GIO<GraphicsBuffer> spConstantBuffer;

	D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	ShaderManager ShaderMgr;

	GraphicsViewPort ViewPort;
	Matrix ProjectionMatrix;
	Matrix ViewMatrix;

	std::map<Model* const, std::vector<ModelInstance>> ItemsToDraw;

	void InitConstantBuffers();
	void InitAdapter();
	void InitFactory();
	void InitDevice();
	void InitSwapChain(const App& arApp);
	void InitRenderTargets();
	void InitDepthStencil(const App& arApp);

	void BeginFrame();
	void EndFrame();
	void PresentFrame();

	void ValidateDevice();
};
