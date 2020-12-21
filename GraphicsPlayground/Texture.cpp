#include "Texture.h"
#include "Graphics.h"
#include "Release.h"

#include <codecvt>
#include <locale>
#include <stdexcept>
#include <WICTextureLoader.h>


Texture::~Texture()
{
	Common::Release<ShaderResourceView>(pTextureView);
	Common::Release<ID3D11SamplerState>(pSamplerState);
}

void Texture::Init(Graphics& arGraphics, const std::wstring& arName)
{
	using namespace DirectX;
	auto& rdevice = arGraphics.GetDevice();
	auto& rctx = arGraphics.GetDeviceCtx();
	auto hr = CreateWICTextureFromFile(&rdevice, &rctx, arName.c_str(), nullptr, &pTextureView);
	if (FAILED(hr))
	{
		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;
		std::string converted_str = converter.to_bytes(L"Failed to load texture " + arName);
		throw new std::runtime_error(converted_str.c_str());
	}

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = rdevice.CreateSamplerState(&sampDesc, &pSamplerState);

	if (FAILED(hr))
	{
		using convert_type = std::codecvt_utf8<wchar_t>;
		std::wstring_convert<convert_type, wchar_t> converter;
		std::string converted_str = converter.to_bytes(L"Failed to create sampler state for texture " + arName);
		throw new std::runtime_error(converted_str.c_str());
	}
}
