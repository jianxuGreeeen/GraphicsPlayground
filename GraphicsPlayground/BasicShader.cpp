#include "BasicShader.h"
#include "Graphics.h"
#include "Release.h"
#include "ShaderHelper.h"
#include "ShaderNames.h"

#include <stdexcept>

// TODO: 
// Split creation of vertices and compiling shaders into two steps.
// This way, multiple shaders can reuse the same compiled data if needed.

using namespace DirectX;

namespace
{
	ShaderLayout Layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
}

void BasicShader::InitPixelShader(Graphics& arGraphics)
{
	auto& rdevice = arGraphics.GetDevice();

	auto* pcso = ShaderHelper::CreateCompiledShaderObject(ShaderNames::BasicPixelShader);
	auto hr = rdevice.CreatePixelShader(pcso->GetBufferPointer(), pcso->GetBufferSize(), nullptr, &pPShader);
	pcso->Release();
	if (FAILED(hr))
	{
		throw new std::runtime_error("BasicShader::Init failed to CreatePixelShader");
	}
}

void BasicShader::InitVertexShader(Graphics& arGraphics)
{
	auto& rdevice = arGraphics.GetDevice();

	auto* pcso = ShaderHelper::CreateCompiledShaderObject(ShaderNames::BasicVertShader);
	auto hr = rdevice.CreateVertexShader(pcso->GetBufferPointer(), pcso->GetBufferSize(), nullptr, &pVShader);
	if (FAILED(hr))
	{
		pcso->Release();
		throw new std::runtime_error("BasicShader::Init failed to CreateVertexShader");
	}

	hr = rdevice.CreateInputLayout(Layout, ARRAYSIZE(Layout),
		pcso->GetBufferPointer(), pcso->GetBufferSize(), &pLayout);

	pcso->Release();
	if (FAILED(hr))
	{
		throw new std::runtime_error("BasicShader::Init failed to CreateInputLayout");
	}

	GraphicsBufferDesc desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(Matrix);
	desc.CPUAccessFlags = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;

	pVCBuffer = arGraphics.CreateBuffer(desc, nullptr);
}

void BasicShader::Init(Graphics& arGraphics)
{
	InitVertexShader(arGraphics);
	InitPixelShader(arGraphics);	
}

void BasicShader::Update(Graphics& arGraphics)
{
	auto& rctx = arGraphics.GetDeviceCtx();
	rctx.VSSetShader(pVShader, nullptr, 0);
	rctx.PSSetShader(pPShader, nullptr, 0);
	rctx.IASetInputLayout(pLayout);
	rctx.VSSetConstantBuffers(0, 1, &pVCBuffer);
}

void BasicShader::UpdateCBuffers(Graphics& arGraphics, const Matrix& arWVPMatrix)
{
	auto& rctx = arGraphics.GetDeviceCtx();

	const auto transposedWvp = XMMatrixTranspose(arWVPMatrix);
	rctx.UpdateSubresource(pVCBuffer, 0, nullptr, &transposedWvp, 0, 0);
}

void BasicShader::Release()
{
	Common::Release<VertexShader>(pVShader);
	Common::Release<ShaderInputLayout>(pLayout);
	Common::Release<PixelShader>(pPShader);
	Common::Release<GraphicsBuffer>(pVCBuffer);
}
