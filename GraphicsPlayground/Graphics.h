#pragma once

#include "Camera.h"
#include "GraphicsInterfaceObject.h"
#include "GraphicsTypes.h"
#include "Model.h"
#include "ShaderManager.h"

#include <d3d11.h>
#include <dxgi1_4.h>
#include <map>

struct ModelInstance
{
	//TODO: Add instance data here
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

	void LoadShaders(const std::vector<ShaderName>& arPixelShaders, const std::vector<ShaderName>& arVertexShaders);
	void AddItemToDraw(Model* apModel, const ModelInstance& arInstanceData);

	GraphicsDevice& GetDevice() { return *spDevice; }
	ShaderManager& GetShaderMgr() { return ShaderMgr; }

private:
	Graphics(Graphics&) = delete;
	Graphics(Graphics&&) = delete;
	Graphics& operator = (Graphics&) = delete;
	Graphics& operator = (Graphics&&) = delete;

#ifdef _DEBUG
	GraphicsInterfaceObject<GraphicsDebug> spDebug;
#endif // _DEBUG

	GraphicsInterfaceObject<GraphicsFactory> spFactory;
	GraphicsInterfaceObject<GraphicsAdapter> spAdapter;
	GraphicsInterfaceObject<GraphicsDevice> spDevice;
	GraphicsInterfaceObject<GraphicsDeviceContext> spDeviceCtx;
	GraphicsInterfaceObject<SwapChain> spSwapChain;
	GraphicsInterfaceObject<RenderTarget> spRenderTarget;

	D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	Camera Cam;
	ShaderManager ShaderMgr;

	std::map<Model*, ModelInstance> ItemsToDraw;

	void InitAdapter();
	void InitFactory();
	void InitDevice();
	void InitSwapChain(const App& arApp);
	void InitRenderTargets();

	void BeginFrame();
	void EndFrame();
	void PresentFrame();

	void ValidateDevice();
};
