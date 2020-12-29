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

	
}
