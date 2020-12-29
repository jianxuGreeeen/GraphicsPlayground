#pragma once
#include <string>
using ShaderName = std::wstring;

namespace ShaderNames
{
	static const ShaderName BasicVertShader = L"BasicVertexShader";
	static const ShaderName BasicPixelShader = L"BasicPixelShader";
	static const ShaderName PbrVertShader = L"PbrVertexShader";
	static const ShaderName PbrPixelShader = L"PbrPixelShader";
};
