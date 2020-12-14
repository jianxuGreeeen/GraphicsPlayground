#include "Graphics.h"
#include <stdexcept>
#include "App.h"

void Graphics::Init()
{
    InitDebug();
    InitFactory();
    InitAdapter();
    InitDevice();
}

void Graphics::PrepForWindow(const App& arApp)
{
    InitCmdQueue();
    InitSwapChain(arApp);
    InitRenderTargets();
    InitCmdAllocators();
    InitCmdList();
    InitFences();
}

void Graphics::Update()
{
}

void Graphics::Draw()
{
    WaitForPreviousFrame();
    BeginFrame();
    

    EndFrame();

    PresentFrame();
}

void Graphics::Shutdown()
{
    WaitForPreviousFrame();
    CloseHandle(FenceEvent);

    for (auto& rfence : spFences)
    {
        rfence.Release();
    }

    spCmdList.Release();
    for (auto& rcmdAlloc: spCmdAllocators)
    {
        rcmdAlloc.Release();
    }

    for (auto& rrenderTargets : spRenderTargets)
    {
        rrenderTargets.Release();
    }
    spSwapChain.Release();
    spCmdQueue.Release();

    spDevice.Release();
    spAdapter.Release();
    spFactory.Release();

#ifdef _DEBUG
    spDebug.Release();
#endif // _DEBUG
}

void Graphics::InitAdapter()
{    
    IDXGIAdapter1* padapter = nullptr; // adapters are the graphics card (this includes the embedded graphics on the motherboard)
    int adapterIndex = 0;

    while (spFactory->EnumAdapters1(adapterIndex, &padapter) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_ADAPTER_DESC1 desc;
        padapter->GetDesc1(&desc);

        const bool isSoftware = desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE;
        if (!isSoftware)
        {
            auto hr = D3D12CreateDevice(padapter, FeatureLevel, _uuidof(ID3D12Device), nullptr);
            if (SUCCEEDED(hr))
            {
                break;
            }
        }
        ++adapterIndex;
        padapter->Release();
        padapter = nullptr;
    }

    if (padapter == nullptr)
    {
        throw std::runtime_error("Failed to init graphics adapter");
    }
    else
    {
        spAdapter = padapter;
    }
}

void Graphics::InitFactory()
{
    IDXGIFactory4* pfactory;
    auto hr = CreateDXGIFactory2(FactoryFlags, IID_PPV_ARGS(&pfactory));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to init graphics factory");
    }
    else
    {
        spFactory = pfactory;
    }
}

void Graphics::InitDevice()
{
    ID3D12Device* pdevice;
    auto hr = D3D12CreateDevice(spAdapter.Get(), FeatureLevel, IID_PPV_ARGS(&pdevice));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to init graphics device");
    }
    else
    {
        spDevice = pdevice;
    }
}

void Graphics::InitDebug()
{
    FactoryFlags &= ~DXGI_CREATE_FACTORY_DEBUG;
#if defined _DEBUG
    ID3D12Debug* pdebug;
    auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(&pdebug));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to init graphics debug");
    }
    else
    {
        spDebug = pdebug;
        spDebug->EnableDebugLayer();
        FactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
    }
#endif // _DEBUG
}

void Graphics::InitCmdQueue()
{
    D3D12_COMMAND_QUEUE_DESC cqDesc = {};
    ID3D12CommandQueue* pcmdQueue = nullptr;
    auto hr = spDevice->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&pcmdQueue)); 
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to init graphics command queue");
    }
    else
    {
        spCmdQueue = pcmdQueue;
    }
}

void Graphics::InitSwapChain(const App& arApp)
{
    auto& rwindowSettings = arApp.GetSettings();

    DXGI_MODE_DESC modeDesc = {};
    modeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //32 bits total.
    modeDesc.Width = rwindowSettings.Width;
    modeDesc.Height = rwindowSettings.Height;

    // no multi sampling for now
    DXGI_SAMPLE_DESC sampleDesc = {};
    sampleDesc.Count = 1;

    DXGI_SWAP_CHAIN_DESC desc = {};
    desc.BufferCount = NumBuffers;
    desc.BufferDesc = modeDesc;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.OutputWindow = arApp.GetWnd();
    desc.SampleDesc = sampleDesc;
    desc.Windowed = true; // TODO : customizable setting

    IDXGISwapChain* pswapChain = nullptr;
    auto hr = spFactory->CreateSwapChain(spCmdQueue.Get(), &desc, &pswapChain);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to init graphics swap chains");
    }
    else
    {
        spSwapChain = static_cast<IDXGISwapChain3*>(pswapChain);
        FrameIndex = spSwapChain->GetCurrentBackBufferIndex();
    }
}

void Graphics::InitRenderTargets()
{
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    desc.NumDescriptors = NumBuffers;

    ID3D12DescriptorHeap* prtvHeap = nullptr;
    auto hr = spDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&prtvHeap));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to init graphics render target descriptor heap");
    }
    else
    {
        spRtvHeap = prtvHeap;
        rtvDescriptorSize = spDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = spRtvHeap->GetCPUDescriptorHandleForHeapStart();
        for (int i = 0; i < NumBuffers; ++i)
        {
            ID3D12Resource* presource = nullptr;
            hr = spSwapChain->GetBuffer(i, IID_PPV_ARGS(&presource));
            if (FAILED(hr))
            {
                throw std::runtime_error("Failed to get graphics render target buffer");
            }
            else
            {
                spRenderTargets[i] = presource;
                spDevice->CreateRenderTargetView(presource, nullptr, rtvHandle);
                rtvHandle.ptr += rtvDescriptorSize;
            }
        }
    }
}

void Graphics::InitCmdAllocators()
{
    for (auto& rcmdAlloc : spCmdAllocators)
    {
        ID3D12CommandAllocator* palloc = nullptr;
        auto hr = spDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&palloc));
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to init graphics command allocators");
        }
        else
        {
            rcmdAlloc = palloc;
        }
    }
}

void Graphics::InitCmdList()
{
    ID3D12GraphicsCommandList* pcmdList = nullptr;
    auto hr = spDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
        spCmdAllocators[0].Get(), nullptr, IID_PPV_ARGS(&pcmdList));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to init graphics command list");
    }
    else
    {
        spCmdList = pcmdList;
        spCmdList->Close(); // command lists are created in the recording state. Turn it off on init.
    }
}

void Graphics::InitFences()
{
    for (auto& rfence : spFences)
    {
        ID3D12Fence* pfence = nullptr;
        auto hr = spDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pfence));
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to init graphics fences");
        }
        else
        {
            rfence = pfence;
        }
    }

    FenceEvent = CreateEvent(nullptr, false, false, nullptr);
    if (FenceEvent == nullptr)
    {
        throw std::runtime_error("Failed to init graphics fence event");
    }
}

void Graphics::WaitForPreviousFrame()
{
    FrameIndex = spSwapChain->GetCurrentBackBufferIndex();

    // fence values does not match, we need to wait for gpu to finish
    if (spFences[FrameIndex]->GetCompletedValue() < FenceValue[FrameIndex])
    {
        // wait for a signal
        auto hr = spFences[FrameIndex]->SetEventOnCompletion(FenceValue[FrameIndex], FenceEvent);
        if (FAILED(hr))
        {
            throw std::runtime_error("Failed to wait for previous frame event completion");
        }

        // We will wait until the fence has triggered the event
        WaitForSingleObject(FenceEvent, INFINITE);
    }

    // increment fenceValue for next frame
    ++FenceValue[FrameIndex];
}

void Graphics::BeginFrame()
{
    auto hr = spCmdAllocators[FrameIndex]->Reset();
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to reset command allocator");
    }

    hr = spCmdList->Reset(spCmdAllocators[FrameIndex].Get(), nullptr);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to reset command list");
    }

    {
        D3D12_RESOURCE_BARRIER barrier;
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = spRenderTargets[FrameIndex].Get();
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        spCmdList->ResourceBarrier(1, &barrier);
    }
    
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = spRtvHeap->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += (FrameIndex * rtvDescriptorSize);

    spCmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

    const float clearColor[] = { 0.53f, 0.77f, 0.96f, 1.0f };
    spCmdList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

void Graphics::EndFrame()
{
    {
        D3D12_RESOURCE_BARRIER barrier;
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = spRenderTargets[FrameIndex].Get();
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        spCmdList->ResourceBarrier(1, &barrier);
    }

    auto hr = spCmdList->Close();
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to end graphics frame");
    }
}

void Graphics::PresentFrame()
{
    ID3D12CommandList* ppCommandLists[] = { spCmdList.Get() };
    spCmdQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // this command goes in at the end of our command queue. we will know when our command queue 
    // has finished because the fence value will be set to "fenceValue" from the GPU since the command
    // queue is being executed on the GPU
    auto hr = spCmdQueue->Signal(spFences[FrameIndex].Get(), FenceValue[FrameIndex]);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to signal graphics command queue");
    }

    // present the current backbuffer
    hr = spSwapChain->Present(0, 0);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to present graphics frame");
    }
}
