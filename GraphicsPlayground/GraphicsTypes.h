#pragma once

struct IUnknown;
#include <d3dcommon.h>
#include <d3d11.h>
#include <dxgiformat.h>

using CompiledShaderObject = ID3DBlob;
using GraphicsAdapter = IDXGIAdapter;
using GraphicsBuffer = ID3D11Buffer;
using GraphicsBufferDesc = D3D11_BUFFER_DESC;
using GraphicsBufferData = D3D11_SUBRESOURCE_DATA;
using GraphicsDebug = ID3D11Debug;
using GraphicsDepthStencil = ID3D11DepthStencilView;
using GraphicsDepthStencilBuffer = ID3D11Texture2D;
using GraphicsDevice = ID3D11Device;
using GraphicsDeviceContext = ID3D11DeviceContext;
using GraphicsFactory = IDXGIFactory;
using GraphicsViewPort = D3D11_VIEWPORT;

using Index = DWORD;
using IndexBuffer = ID3D11Buffer;
using PixelShader = ID3D11PixelShader;
using RenderTarget = ID3D11RenderTargetView;
using ShaderInputLayout = ID3D11InputLayout;
using ShaderLayout = D3D11_INPUT_ELEMENT_DESC;
using SwapChain = IDXGISwapChain;
using SwapChainBuffer = ID3D11Texture2D;
using VertexShader = ID3D11VertexShader;
using VertexBuffer = ID3D11Buffer;

