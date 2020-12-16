#pragma once

struct IUnknown;
#include <d3dcommon.h>
#include <d3d11.h>
#include <dxgiformat.h>

using GraphicsAdapter = IDXGIAdapter;
using GraphicsDebug = ID3D11Debug;
using GraphicsDevice = ID3D11Device;
using GraphicsDeviceContext = ID3D11DeviceContext;
using GraphicsFactory = IDXGIFactory;
using CompiledShaderObject = ID3DBlob;
using RenderTarget = ID3D11RenderTargetView;
using ShaderLayout = D3D11_INPUT_ELEMENT_DESC;
using SwapChain = IDXGISwapChain;
using SwapChainBuffer = ID3D11Texture2D;
using VertexShader = ID3D11VertexShader;
using PixelShader = ID3D11PixelShader;
using VertexBuffer = ID3D11Buffer;
using VertexLayout = ID3D11InputLayout;

