#include "Graphics.h"
#include <stdexcept>
#include "App.h"
#include "Model.h"

namespace
{
    constexpr D3D_FEATURE_LEVEL FeatureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_1
    };
    constexpr int NumFeatureLevels = 5;
}

void Graphics::Init()
{
    InitFactory();
    InitAdapter();
    InitDevice();
}

void Graphics::PrepForWindow(const App& arApp)
{
    InitSwapChain(arApp);
    InitRenderTargets();

    Cam.VP.Width = static_cast<float>(arApp.GetSettings().Width);
    Cam.VP.Height = static_cast<float>(arApp.GetSettings().Height);
}

void Graphics::LoadShaders(const std::vector<ShaderName>& arPixelShaders, const std::vector<ShaderName>& arVertexShaders)
{
    ValidateDevice();
    ShaderMgr.Load(*spDevice, arPixelShaders, arVertexShaders);
}

void Graphics::AddItemToDraw(Model* apModel, const ModelInstance& arInstanceData)
{
    ValidateDevice();
    ItemsToDraw.emplace(std::make_pair(apModel, arInstanceData));
}

void Graphics::Update()
{
    ItemsToDraw.clear();
}

void Graphics::Draw()
{
    BeginFrame();

    //Create the Viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = Cam.VP.Width;
    viewport.Height = Cam.VP.Height;

    //Set the Viewport
    spDeviceCtx->RSSetViewports(1, &viewport);

    for (auto& kvp : ItemsToDraw)
    {
        auto* pmodel = kvp.first;
        auto* pvbuffer = pmodel->GetVBuffer();

        auto& rvshaderData = ShaderMgr.GetVs(pmodel->GetVShader());
        auto& rvshader = *(rvshaderData.Vs);
        auto& rvlayout = *(rvshaderData.VsLayout);
        auto& rpshader = ShaderMgr.GetPs(pmodel->GetPShader());

        spDeviceCtx->VSSetShader(&rvshader, nullptr, 0);
        spDeviceCtx->PSSetShader(&rpshader, nullptr, 0);

        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        spDeviceCtx->IASetVertexBuffers(0, 1, &pvbuffer, &stride, &offset);
        spDeviceCtx->IASetInputLayout(&rvlayout);
        spDeviceCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        const auto numVerts = static_cast<UINT>(pmodel->GetVerts().size());
        spDeviceCtx->Draw(numVerts, 0);
    }

    EndFrame();
    PresentFrame();
}

void Graphics::Shutdown()
{
}

void Graphics::InitAdapter()
{    
    GraphicsAdapter* padapter = nullptr; // adapters are the graphics card (this includes the embedded graphics on the motherboard)
    int adapterIndex = 0;
    while (spFactory->EnumAdapters(adapterIndex, &padapter) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_ADAPTER_DESC desc;
        padapter->GetDesc(&desc);

        const bool isHardware = desc.DedicatedVideoMemory > 0;
        if (isHardware)
        {
            break;
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
    GraphicsFactory* pfactory;
    auto hr = CreateDXGIFactory(IID_PPV_ARGS(&pfactory));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed CreateDXGIFactory2");
    }
    else
    {
        spFactory = pfactory;
    }
}

void Graphics::InitDevice()
{
    UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif //_DEBUG

    GraphicsDevice* pdevice = nullptr;
    GraphicsDeviceContext* pdeviceCtx = nullptr;
    auto hr = D3D11CreateDevice(spAdapter.Get(), 
        D3D_DRIVER_TYPE_UNKNOWN,
        nullptr, //no software
        flags,
        FeatureLevels,
        NumFeatureLevels,
        D3D11_SDK_VERSION,
        &pdevice,
        &FeatureLevel,
        &pdeviceCtx);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed D3D11CreateDevice");
    }
    else
    {
        spDevice = pdevice;
        spDeviceCtx = pdeviceCtx;
    }
}

void Graphics::InitSwapChain(const App& arApp)
{
    auto& rwindowSettings = arApp.GetSettings();

    // TODO : customizable setting
    DXGI_MODE_DESC bufferDesc = {};
    bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //32 bits total.
    bufferDesc.Width = rwindowSettings.Width;
    bufferDesc.Height = rwindowSettings.Height;
    bufferDesc.RefreshRate.Numerator = 60;
    bufferDesc.RefreshRate.Denominator = 1;
    bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    // TODO : customizable setting
    // no multi sampling for now
    DXGI_SAMPLE_DESC sampleDesc = {};
    sampleDesc.Count = 1;

    DXGI_SWAP_CHAIN_DESC desc = {};
    desc.BufferCount = 2; // triple buffer
    desc.BufferDesc = bufferDesc;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.OutputWindow = arApp.GetWnd();
    desc.SampleDesc = sampleDesc;
    desc.Windowed = true; // TODO : customizable setting

    IDXGISwapChain* pswapChain = nullptr;
    auto hr = spFactory->CreateSwapChain(spDevice.Get(), &desc, &pswapChain);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed spFactory->CreateSwapChain");
    }
    else
    {
        spSwapChain = static_cast<SwapChain*>(pswapChain);
    }
}

void Graphics::InitRenderTargets()
{
    SwapChainBuffer* pbuffer = nullptr;
    auto hr = spSwapChain->GetBuffer(0, IID_PPV_ARGS(&pbuffer));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed spSwapChain->GetBuffer");
    }
    else
    {
        RenderTarget* prt = nullptr;
        hr = spDevice->CreateRenderTargetView(pbuffer, nullptr, &prt);
        pbuffer->Release();

        if (FAILED(hr))
        {
            throw std::runtime_error("Failed CreateRenderTargetView");
        }
        else
        {
            spRenderTarget = prt;
        }
    }
}

void Graphics::BeginFrame()
{
    constexpr float clearColor[] = { 0.53f, 0.77f, 0.96f, 1.0f };

    //Set our Render Target
    auto* prtv = spRenderTarget.Get();
    spDeviceCtx->OMSetRenderTargets(1, &prtv, nullptr);
    spDeviceCtx->ClearRenderTargetView(prtv, clearColor);
}

void Graphics::EndFrame()
{
   /* {
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
    }*/
}

void Graphics::PresentFrame()
{
    // present the current backbuffer
    const auto hr = spSwapChain->Present(0, 0);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to present graphics frame");
    }
}

void Graphics::ValidateDevice()
{
    if (spDevice.Get() == nullptr)
    {
        throw new std::runtime_error("Cannot load shaders without a device set");
    }
}
