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
	PShaderKey PShader = PShaderKey::BasicShader;
	VShaderKey VShader = VShaderKey::BasicShader;
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
	void Update(const App& arApp);
	void Draw();
	void Shutdown();

	void LoadResources();
	void AddItemToDraw(Model* const apModel, const ModelInstance& arInstanceData);

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
	GraphicsInterfaceObject<GraphicsDepthStencil> spDepthStencil;
	GraphicsInterfaceObject<GraphicsDepthStencilBuffer> spDepthStencilBuffer;

	std::array<GraphicsBuffer*, 3> ConstantBuffers;

	D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	Camera Cam;
	ShaderManager ShaderMgr;

	Matrix ProjectionMatrix;
	Matrix ViewMatrix;
	Matrix WorldMatrix;

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
	void UpdateCBufferData(std::array< GlobalDataType, 3>& arUpdateInfo);
};
