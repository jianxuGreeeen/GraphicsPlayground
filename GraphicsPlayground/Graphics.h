#pragma once
struct IUnknown;
#include <memory>
#include <d3d12.h>
#include <dxgi1_4.h>

class App;

/*
* Since direct 3d objects needs to be released,
* I didn't want to keep managing release calls all over.
* This is a wrapper class to do it for me.
*/
template <class T>
class GraphicsInterfaceObject final
{
public:
	void Release()
	{
		if (Ptr != nullptr)
		{
			Ptr->Release();
			Ptr = nullptr;
		}
	}

	~GraphicsInterfaceObject()
	{
		Release();
	}

	GraphicsInterfaceObject& operator = (T* aPtr)
	{
		Release();
		Ptr = aPtr;
		return *this;
	}

	T* operator -> ()
	{
		return Get();
	}

	T* Get() { return Ptr; }

private:
	T* Ptr = nullptr;
};

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

private:
	Graphics(Graphics&) = delete;
	Graphics(Graphics&&) = delete;
	Graphics& operator = (Graphics&) = delete;
	Graphics& operator = (Graphics&&) = delete;

	static const int NumBuffers = 3;

#ifdef _DEBUG
	GraphicsInterfaceObject<ID3D12Debug> spDebug;
#endif // _DEBUG

	GraphicsInterfaceObject<IDXGIFactory4> spFactory;
	GraphicsInterfaceObject<IDXGIAdapter1> spAdapter;
	GraphicsInterfaceObject<ID3D12Device> spDevice;
	GraphicsInterfaceObject<ID3D12CommandQueue> spCmdQueue;
	GraphicsInterfaceObject<IDXGISwapChain3> spSwapChain;


	GraphicsInterfaceObject<ID3D12DescriptorHeap> spRtvHeap;
	GraphicsInterfaceObject<ID3D12Resource> spRenderTargets[NumBuffers];
	GraphicsInterfaceObject<ID3D12CommandAllocator> spCmdAllocators[NumBuffers];
	GraphicsInterfaceObject<ID3D12GraphicsCommandList> spCmdList; 
	GraphicsInterfaceObject<ID3D12Fence> spFences[NumBuffers];   
	uint64_t FenceValue[NumBuffers];
	HANDLE FenceEvent;

	
	//int frameIndex; // current rtv we are on

	//int rtvDescriptorSize; // size of the rtv descriptor on the device (all front and back buffers will be the same size)

	D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;
	UINT FactoryFlags = 0u;
	int64_t FrameIndex = 0;
	int64_t rtvDescriptorSize = 0;

	void InitAdapter();
	void InitFactory();
	void InitDevice();
	void InitDebug();
	void InitCmdQueue();
	void InitSwapChain(const App& arApp);
	void InitRenderTargets();
	void InitCmdAllocators();
	void InitCmdList();
	void InitFences();

	void WaitForPreviousFrame();
	void BeginFrame();
	void EndFrame();
	void PresentFrame();
};
