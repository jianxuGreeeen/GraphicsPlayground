#include "Graphics.h"
#include "App.h"
#include "IShader.h"
#include "Light.h"
#include "Release.h"

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

    DirectionalLight DirLight = {
            Color4(.2f, .2f, .2f, 1.0f),
            Float3(1.0f, -1.0f, 0.0f)
    };
}

void Graphics::Init(const App& arApp)
{
    InitFactory();
    InitAdapter();
    InitDevice();
    InitSwapChain(arApp);
    InitRenderTargets();
    InitDepthStencil(arApp);
    InitRasterizerStates();

    //Create the Viewport
    ZeroMemory(&ViewPort, sizeof(GraphicsViewPort));
    ViewPort.TopLeftX = 0;
    ViewPort.TopLeftY = 0;
    ViewPort.Height = static_cast<float>(arApp.GetSettings().Height);
    ViewPort.Width = static_cast<float>(arApp.GetSettings().Width);
    ViewPort.MinDepth = 0.0f;
    ViewPort.MaxDepth = 1.0f;

    auto& rwindowSettings = arApp.GetSettings();
    const auto width = static_cast<float>(rwindowSettings.Width);
    const auto height = static_cast<float>(rwindowSettings.Height);
    AspectRatio = width / height;

    DirLight = {
        Color4(0.0f, 0.0f, 1.0f, 1.0f),
        Float3(1.0f, -1.0f, 0.0f)
    };
}

void Graphics::AddItemToDraw(const GraphicsDrawState& arDrawState, const ModelInstance& arInstanceData)
{
    ValidateDevice();
    ItemsToDraw[arDrawState].push_back(arInstanceData);
}

void Graphics::AddPointLight(const PointLight& arInstance)
{
    ValidateDevice();
    PointLight1 = arInstance;
}

void Graphics::SetCamera(const Camera& arCamera)
{
    Cam = arCamera;

    using namespace DirectX;
    const Vector camUp = XMVectorSet(0, 1, 0, 0);
    ViewMatrix = XMMatrixLookAtLH(Cam.EyePos, Cam.FocusPos, camUp);
    ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(Cam.Fov), AspectRatio, Cam.Near, Cam.Far);
}

void Graphics::Update()
{
    ItemsToDraw.clear();
}

void Graphics::Draw()
{
    BeginFrame();    

    using namespace DirectX;

    // TODO : allow models to select shader to use
    for (auto& kvp : ItemsToDraw)
    {
        auto& rdrawState = kvp.first;
        auto* pshader = rdrawState.pShader;
        auto rasterizeState = rdrawState.RasterizerState;
        pDeviceCtx->RSSetState(pRasterizerStates[static_cast<int>(rasterizeState)]);
        pshader->Update(*this);

        auto* pmodel = rdrawState.pModel;
        pmodel->Update(*this);

        // per frame data
        pshader->UpdateCBuffers(*this, ShaderBufferConstants::DirLight, &DirLight);
        pshader->UpdateCBuffers(*this, ShaderBufferConstants::PointLight, &PointLight1);
        pshader->UpdateCBuffers(*this, ShaderBufferConstants::CamPos, &Cam.EyePos);

        auto& instances = kvp.second;
        for (auto& rinstance : instances)
        {
            const auto wvp = rinstance.WorldMatrix * ViewMatrix * ProjectionMatrix;
            const auto transposedWvp = XMMatrixTranspose(wvp);
            const auto world = XMMatrixTranspose(rinstance.WorldMatrix);

            // per object data
            pshader->UpdateCBuffers(*this, ShaderBufferConstants::WVP, &transposedWvp);
            pshader->UpdateCBuffers(*this, ShaderBufferConstants::World, &world);

            //commit to gpu
            pshader->CommitCBufferData(*this);

            for (auto& rtextures : rinstance.pTextures)
            {
                pshader->UpdateTexture(*this, rtextures.first, rtextures.second);
            }
            
            pDeviceCtx->DrawIndexed(pmodel->NumIndices(), 0, 0);
        }
    }

    EndFrame();
    PresentFrame();
}

void Graphics::Shutdown()
{
    for (auto*& prasterizerState : pRasterizerStates)
    {
        Common::Release<GraphicsRasterizerState>(prasterizerState);
    }

    Common::Release<GraphicsDepthStencilBuffer>(pDepthStencilBuffer);
    Common::Release<GraphicsDepthStencil>(pDepthStencil);

    Common::Release<RenderTarget>(pRenderTarget);
    Common::Release<SwapChain>(pSwapChain);

    Common::Release<GraphicsDeviceContext>(pDeviceCtx);
    Common::Release<GraphicsDevice>(pDevice);
    Common::Release<GraphicsAdapter>(pAdapter);
    Common::Release<GraphicsFactory>(pFactory);
}

void Graphics::InitAdapter()
{    
    GraphicsAdapter* padapter = nullptr; // adapters are the graphics card (this includes the embedded graphics on the motherboard)
    int adapterIndex = 0;
    while (pFactory->EnumAdapters(adapterIndex, &padapter) != DXGI_ERROR_NOT_FOUND)
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
    pAdapter = padapter;
}

void Graphics::InitFactory()
{
    GraphicsFactory* pfactory;
    auto hr = CreateDXGIFactory(IID_PPV_ARGS(&pfactory));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed CreateDXGIFactory2");
    }
    pFactory = pfactory;
}

void Graphics::InitDevice()
{
    UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif //_DEBUG

    GraphicsDevice* pdevice = nullptr;
    GraphicsDeviceContext* pdeviceCtx = nullptr;
    auto hr = D3D11CreateDevice(pAdapter, 
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
    
    pDevice = pdevice;
    pDeviceCtx = pdeviceCtx;
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
    auto hr = pFactory->CreateSwapChain(pDevice, &desc, &pswapChain);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed spFactory->CreateSwapChain");
    }
    pSwapChain = static_cast<SwapChain*>(pswapChain);
}

void Graphics::InitRenderTargets()
{
    SwapChainBuffer* pbuffer = nullptr;
    auto hr = pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pbuffer));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed spSwapChain->GetBuffer");
    }

    RenderTarget* prt = nullptr;
    hr = pDevice->CreateRenderTargetView(pbuffer, nullptr, &prt);
    pbuffer->Release();

    if (FAILED(hr))
    {
        throw std::runtime_error("Failed CreateRenderTargetView");
    }
    pRenderTarget = prt;
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
    auto hr = pDevice->CreateTexture2D(&desc, nullptr, &pbuffer);
    if (FAILED(hr))
    {
        throw new std::runtime_error("InitDepthStencil failed CreateTexture2D");
    }
    pDepthStencilBuffer = pbuffer;

    GraphicsDepthStencil* pdepthStencil = nullptr;
    hr = pDevice->CreateDepthStencilView(pbuffer, nullptr, &pdepthStencil);
    if (FAILED(hr))
    {
        throw new std::runtime_error("InitDepthStencil failed CreateDepthStencilView");
    }
    pDepthStencil = pdepthStencil;
}

void Graphics::InitRasterizerStates()
{
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

    auto hr = pDevice->CreateRasterizerState(&rasterizerDesc, &pRasterizerStates[static_cast<int>(RasterizerStates::Default)]);
    if (FAILED(hr))
    {
        throw new std::runtime_error("Failed CreateRasterizerState with RasterizerStates::Default");
    }

    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;

    hr = pDevice->CreateRasterizerState(&rasterizerDesc, &pRasterizerStates[static_cast<int>(RasterizerStates::WireFrame)]);
    if (FAILED(hr))
    {
        throw new std::runtime_error("Failed CreateRasterizerState with RasterizerStates::WireFrame");
    }
}

void Graphics::BeginFrame()
{
    constexpr float clearColor[] = { 0.53f, 0.77f, 0.96f, 1.0f };

    //Set our Render Target
    pDeviceCtx->ClearRenderTargetView(pRenderTarget, clearColor);
    pDeviceCtx->OMSetRenderTargets(1, &pRenderTarget, pDepthStencil);
    pDeviceCtx->ClearDepthStencilView(pDepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    pDeviceCtx->RSSetViewports(1, &ViewPort);
}

void Graphics::EndFrame()
{
}

void Graphics::PresentFrame()
{
    // present the current backbuffer
    const auto hr = pSwapChain->Present(0, 0);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to present graphics frame");
    }
}

void Graphics::ValidateDevice()
{
    if (pDevice == nullptr)
    {
        throw new std::runtime_error("Cannot load shaders without a device set");
    }
}

GraphicsBuffer* Graphics::CreateVertexBuffer(const std::vector<Vertex>& arVerts)
{
    GraphicsBufferDesc desc = {};
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = static_cast<int>(arVerts.size()) * Vertex::ByteWidth;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    GraphicsBufferData data = {};
    data.pSysMem = arVerts.data();

    return CreateBuffer(desc, &data);
}

GraphicsBuffer* Graphics::CreateIndexBuffer(const std::vector<Index>& arIdices)
{
    GraphicsBufferDesc desc = {};
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = static_cast<int>(arIdices.size()) * sizeof(Index);
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    GraphicsBufferData data = {};
    data.pSysMem = arIdices.data();

    return CreateBuffer(desc, &data);
}

GraphicsBuffer* Graphics::CreateBuffer(const GraphicsBufferDesc& arDesc, const GraphicsBufferData* apData)
{
    ValidateDevice();

    GraphicsBuffer* pbuffer = nullptr;
    auto hr = pDevice->CreateBuffer(&arDesc, apData, &pbuffer);
    if (FAILED(hr))
    {
        throw new std::runtime_error("CreateBuffer failed");
    }
    return pbuffer;
}
