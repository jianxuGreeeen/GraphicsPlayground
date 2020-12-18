#include "Graphics.h"
#include "App.h"
#include "Model.h"
#include <DirectXMath.h>
#include <stdexcept>

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

    GraphicsBuffer* CreateBuffer(const GraphicsBufferDesc* apDesc, const GraphicsBufferData* apData, GraphicsDevice& arDevice)
    {
        GraphicsBuffer* pbuffer = nullptr;
        auto hr = arDevice.CreateBuffer(apDesc, apData, &pbuffer);
        if (FAILED(hr))
        {
            throw new std::runtime_error("Graphics CreateBuffer failed");
        }
        return pbuffer;
    }

    // constant buffer indices
    constexpr int ProjMatrixIdx = 0;
    constexpr int ViewMatrixIdx = 1;
    constexpr int WorldMatrixIdx = 2;
}

void Graphics::Init()
{
    InitFactory();
    InitAdapter();
    InitDevice();
    InitConstantBuffers();
}

void Graphics::PrepForWindow(const App& arApp)
{
    InitSwapChain(arApp);
    InitRenderTargets();
    InitDepthStencil(arApp);

    //Create the Viewport
    ZeroMemory(&ViewPort, sizeof(GraphicsViewPort));
    ViewPort.TopLeftX = 0;
    ViewPort.TopLeftY = 0;
    ViewPort.Height = static_cast<float>(arApp.GetSettings().Height);
    ViewPort.Width = static_cast<float>(arApp.GetSettings().Width);
    ViewPort.MinDepth = 0.0f;
    ViewPort.MaxDepth = 1.0f;
}

void Graphics::LoadResources()
{
    ValidateDevice();
    ShaderMgr.Load(*spDevice);
}

void Graphics::AddItemToDraw(Model* const apModel, const ModelInstance& arInstanceData)
{
    ValidateDevice();
    ItemsToDraw[apModel].push_back(arInstanceData);
}

void Graphics::Update()
{
    ItemsToDraw.clear();
}

void Graphics::Draw()
{
    BeginFrame();    

    for (auto& kvp : ItemsToDraw)
    {
        auto* pmodel = kvp.first;
        auto* pvbuffer = pmodel->GetVBuffer();
        auto* pibuffer = pmodel->GetIBuffer();

        auto& rvshaderData = ShaderMgr.GetVShader(pmodel->GetVShader());
        auto& rvshader = *(rvshaderData.spShader.Get());
        auto& rvlayout = *(rvshaderData.spLayout);
        auto& rpshaderData = ShaderMgr.GetPShader(pmodel->GetPShader());
        auto& rpshader = *(rpshaderData.spShader.Get());

        spDeviceCtx->VSSetShader(&rvshader, nullptr, 0);
        spDeviceCtx->PSSetShader(&rpshader, nullptr, 0);

        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        spDeviceCtx->IASetVertexBuffers(0, 1, &pvbuffer, &stride, &offset);
        spDeviceCtx->IASetIndexBuffer(pibuffer, DXGI_FORMAT_R32_UINT, 0);
        spDeviceCtx->IASetInputLayout(&rvlayout);
        spDeviceCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        auto* pcbuffer = spConstantBuffer.Get();
        spDeviceCtx->VSSetConstantBuffers(0, 1, &pcbuffer);

        auto& instances = kvp.second;
        for (auto& rinstance : instances)
        {
            const auto wvp = rinstance.WorldMatrix * ViewMatrix * ProjectionMatrix;
            const auto transposedWvp = DirectX::XMMatrixTranspose(wvp);
            spDeviceCtx->UpdateSubresource(pcbuffer, 0, nullptr, &transposedWvp, 0, 0);
            spDeviceCtx->DrawIndexed(pmodel->NumIndices(), 0, 0);
        }
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
    spAdapter = padapter;
}

void Graphics::InitFactory()
{
    GraphicsFactory* pfactory;
    auto hr = CreateDXGIFactory(IID_PPV_ARGS(&pfactory));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed CreateDXGIFactory2");
    }
    spFactory = pfactory;
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
    
    spDevice = pdevice;
    spDeviceCtx = pdeviceCtx;
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
    desc.BufferCount = 1; // double buffer
    desc.BufferDesc = bufferDesc;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    desc.OutputWindow = arApp.GetWnd();
    desc.SampleDesc = sampleDesc;
    desc.Windowed = true; // TODO : customizable setting

    IDXGISwapChain* pswapChain = nullptr;
    auto hr = spFactory->CreateSwapChain(spDevice.Get(), &desc, &pswapChain);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed spFactory->CreateSwapChain");
    }
    spSwapChain = static_cast<SwapChain*>(pswapChain);
}

void Graphics::InitRenderTargets()
{
    SwapChainBuffer* pbuffer = nullptr;
    auto hr = spSwapChain->GetBuffer(0, IID_PPV_ARGS(&pbuffer));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed spSwapChain->GetBuffer");
    }

    RenderTarget* prt = nullptr;
    hr = spDevice->CreateRenderTargetView(pbuffer, nullptr, &prt);
    pbuffer->Release();

    if (FAILED(hr))
    {
        throw std::runtime_error("Failed CreateRenderTargetView");
    }
    spRenderTarget = prt;
}

void Graphics::InitDepthStencil(const App& arApp)
{
    auto& rwindowSettings = arApp.GetSettings();

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = rwindowSettings.Width;
    desc.Height = rwindowSettings.Height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    GraphicsDepthStencilBuffer* pbuffer = nullptr;
    auto hr = spDevice->CreateTexture2D(&desc, nullptr, &pbuffer);
    if (FAILED(hr))
    {
        throw new std::runtime_error("InitDepthStencil failed CreateTexture2D");
    }
    spDepthStencilBuffer = pbuffer;

    GraphicsDepthStencil* pdepthStencil = nullptr;
    hr = spDevice->CreateDepthStencilView(pbuffer, nullptr, &pdepthStencil);
    if (FAILED(hr))
    {
        throw new std::runtime_error("InitDepthStencil failed CreateDepthStencilView");
    }
    spDepthStencil = pdepthStencil;

    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.AntialiasedLineEnable = false;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0.0f;
    rasterizerDesc.DepthClipEnable = true;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.MultisampleEnable = false;
    rasterizerDesc.ScissorEnable = false;
    rasterizerDesc.SlopeScaledDepthBias = 0.0f;

    GraphicsRasterizerState* prasterizerState = nullptr;
    hr = spDevice->CreateRasterizerState(&rasterizerDesc, &prasterizerState);
    if (FAILED(hr))
    {
        throw new std::runtime_error("InitDepthStencil failed CreateRasterizerState");
    }
    spRasterizerState = prasterizerState;
}

void Graphics::BeginFrame()
{
    constexpr float clearColor[] = { 0.53f, 0.77f, 0.96f, 1.0f };

    //Set our Render Target
    auto* prtv = spRenderTarget.Get();
    spDeviceCtx->ClearRenderTargetView(prtv, clearColor);
    spDeviceCtx->OMSetRenderTargets(1, &prtv, spDepthStencil.Get());
    spDeviceCtx->ClearDepthStencilView(spDepthStencil.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    spDeviceCtx->RSSetState(spRasterizerState.Get());
    spDeviceCtx->RSSetViewports(1, &ViewPort);

}

void Graphics::EndFrame()
{
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

void Graphics::InitConstantBuffers()
{
    D3D11_BUFFER_DESC constantBufferDesc = {};
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.ByteWidth = sizeof(Matrix);
    constantBufferDesc.CPUAccessFlags = 0;
    constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    spConstantBuffer = CreateBuffer(&constantBufferDesc, nullptr, *spDevice);
}
