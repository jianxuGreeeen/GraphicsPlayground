#include "BasicShader.h"
#include "Graphics.h"
#include "Release.h"
#include "ShaderHelper.h"
#include "ShaderNames.h"
#include "Texture.h"

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
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
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

	GraphicsBufferDesc desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(PShaderCBuffer);
	desc.CPUAccessFlags = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;

	pSCBuffer = arGraphics.CreateBuffer(desc, nullptr);
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
	desc.ByteWidth = sizeof(VShaderCBuffer);
	desc.CPUAccessFlags = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;

	pVCBuffer = arGraphics.CreateBuffer(desc, nullptr);
}

void BasicShader::Init(Graphics& arGraphics)
{
	InitVertexShader(arGraphics);
	InitPixelShader(arGraphics);	

	auto& rdevice = arGraphics.GetDevice();

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	auto hr = rdevice.CreateSamplerState(&sampDesc, &pSamplerState);
	if (FAILED(hr))
	{
		throw new std::runtime_error("Failed to create sampler state for BasicShader");
	}
}

void BasicShader::Update(Graphics& arGraphics)
{
	auto& rctx = arGraphics.GetDeviceCtx();
	rctx.VSSetShader(pVShader, nullptr, 0);
	rctx.PSSetShader(pPShader, nullptr, 0);
	rctx.IASetInputLayout(pLayout);
	rctx.VSSetConstantBuffers(0, 1, &pVCBuffer);
	rctx.PSSetConstantBuffers(0, 1, &pSCBuffer);
	rctx.PSSetSamplers(0, 1, &pSamplerState);
}

void BasicShader::UpdateCBuffers(Graphics& arGraphics, const ShaderBufferConstants arKey, const void* apData)
{
	if (arKey == ShaderBufferConstants::WVP)
	{
		VcBufferData.WVP = *(static_cast<const Matrix*>(apData));
	}
	else if (arKey == ShaderBufferConstants::World)
	{
		VcBufferData.World = *(static_cast<const Matrix*>(apData));
	}
	else if (arKey == ShaderBufferConstants::DirLight)
	{
		using namespace DirectX;
		const DirectionalLight& rlight = *(static_cast<const DirectionalLight*>(apData));
		PcBufferData.DirLight.Color = rlight.Color;
		PcBufferData.DirLight.Dir = rlight.Dir;
	}
	else if (arKey == ShaderBufferConstants::PointLight)
	{
		using namespace DirectX;
		const PointLight& rlight = *(static_cast<const PointLight*>(apData));
		PcBufferData.PtLight1.Pos = rlight.Pos;
		PcBufferData.PtLight1.Color = rlight.Color;
		PcBufferData.PtLight1.Attenuation = rlight.Attenuation;
	}
}

void BasicShader::CommitCBufferData(Graphics& arGraphics)
{
	auto& rctx = arGraphics.GetDeviceCtx();
	rctx.UpdateSubresource(pVCBuffer, 0, nullptr, &VcBufferData, 0, 0);
	rctx.UpdateSubresource(pSCBuffer, 0, nullptr, &PcBufferData, 0, 0);
}

void BasicShader::UpdateTexture(Graphics& arGraphics, const TextureKey arKey, Texture* const apData)
{
	if (arKey == TextureKey::ModelTex1)
	{
		auto& rctx = arGraphics.GetDeviceCtx();
		if (apData != nullptr)
		{
			auto* ptextureView = apData->GetTextureView();
			rctx.PSSetShaderResources(0, 1, &ptextureView);
		}
		else
		{
			ShaderResourceView* nullSRV[1] = { nullptr };
			rctx.PSSetShaderResources(0, 1, nullSRV);
		}
	}
}

void BasicShader::Release()
{
	Common::Release<VertexShader>(pVShader);
	Common::Release<ShaderInputLayout>(pLayout);
	Common::Release<PixelShader>(pPShader);
	Common::Release<GraphicsBuffer>(pVCBuffer);
	Common::Release<GraphicsBuffer>(pSCBuffer);
	Common::Release<ID3D11SamplerState>(pSamplerState);
}
