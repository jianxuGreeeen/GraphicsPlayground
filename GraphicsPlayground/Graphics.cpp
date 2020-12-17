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

    Cam.VP.Width = static_cast<float>(arApp.GetSettings().Width);
    Cam.VP.Height = static_cast<float>(arApp.GetSettings().Height);
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

void Graphics::Update(const App& arApp)
{
    ItemsToDraw.clear();

    const auto width = arApp.GetWidth();
    const auto height = arApp.GetHeight();

    using namespace DirectX;
    ProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), width / height, 0.01f, 100.0f);

    XMVECTOR eyePosition = XMVectorSet(0, 0, -1, 1);
    XMVECTOR focusPoint = XMVectorSet(0, 0, 0, 1);
    XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);
    ViewMatrix = XMMatrixLookAtLH(eyePosition, focusPoint, upDirection);


    static float angle = 0.0f;
    XMVECTOR rotationAxis = XMVectorSet(0, 1, 1, 0);
    WorldMatrix = XMMatrixRotationAxis(rotationAxis, XMConvertToRadians(angle));
}

void Graphics::Draw()
{
    BeginFrame();

    //Create the Viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Height = Cam.VP.Height;
    viewport.Width = Cam.VP.Width;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    //Set the Viewport
    spDeviceCtx->RSSetViewports(1, &viewport);
    spDeviceCtx->VSSetConstantBuffers(0, 3, ConstantBuffers.data());

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

        UpdateCBufferData(rvshaderData.cBufferTypes);

        spDeviceCtx->VSSetShader(&rvshader, nullptr, 0);
        spDeviceCtx->PSSetShader(&rpshader, nullptr, 0);

        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        spDeviceCtx->IASetVertexBuffers(0, 1, &pvbuffer, &stride, &offset);
        spDeviceCtx->IASetIndexBuffer(pibuffer, DXGI_FORMAT_R32_UINT, 0);
        spDeviceCtx->IASetInputLayout(&rvlayout);
        spDeviceCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        spDeviceCtx->DrawIndexed(pmodel->NumIndices(), 0, 0);
    }

    EndFrame();
    PresentFrame();
}

void Graphics::Shutdown()
{
    for (auto*& pbuffer : ConstantBuffers)
    {
        if (pbuffer != nullptr)
        {
            pbuffer->Release();
            pbuffer = nullptr;
        }
    }
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

void Graphics::UpdateCBufferData(std::array< GlobalDataType, 3>& arUpdateInfo)
{
    int idx = 0;
    for (auto globalType : arUpdateInfo)
    {
        auto* pcbuffer = ConstantBuffers[idx];

        switch (globalType)
        {
        case GlobalDataType::ProjectionMatrix:
            spDeviceCtx->UpdateSubresource(pcbuffer, 0, nullptr, &ProjectionMatrix, 0, 0);
            break;
        case GlobalDataType::ViewMatrix:
            spDeviceCtx->UpdateSubresource(pcbuffer, 0, nullptr, &ViewMatrix, 0, 0);
            break;
        case GlobalDataType::WorldMatrix:
            spDeviceCtx->UpdateSubresource(pcbuffer, 0, nullptr, &WorldMatrix, 0, 0);
            break;
        case GlobalDataType::None: // same as default, do nothing
        default:
            break;
        }
        ++idx;
    }    
}

void Graphics::InitConstantBuffers()
{
    D3D11_BUFFER_DESC constantBufferDesc = {};
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.ByteWidth = sizeof(Matrix);
    constantBufferDesc.CPUAccessFlags = 0;
    constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

    ConstantBuffers[0] = CreateBuffer(&constantBufferDesc, nullptr, *spDevice);
    ConstantBuffers[1] = CreateBuffer(&constantBufferDesc, nullptr, *spDevice);
    ConstantBuffers[2] = CreateBuffer(&constantBufferDesc, nullptr, *spDevice);
}
