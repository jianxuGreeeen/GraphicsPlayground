#include "ShaderHelper.h"
#include "BasicShader.h"
#include "PbrShader.h"

#include <d3dcompiler.h>
#include <stdexcept>

namespace
{
	const std::wstring ShaderExtension = L".cso";

	std::wstring MakeShaderFileName(const std::wstring& arShaderName)
	{
		return (arShaderName + ShaderExtension);
	}
}

namespace ShaderHelper
{
	CompiledShaderObject* CreateCompiledShaderObject(const ShaderName& arFileName)
	{
		const auto filename = MakeShaderFileName(arFileName);
		CompiledShaderObject* pshader = nullptr; // d3d blob for holding vertex shader bytecode
		auto hr = D3DReadFileToBlob(filename.c_str(), &pshader);
		if (FAILED(hr))
		{
			throw std::runtime_error("CompiledShaderObject failed to read from file");
		}
		return pshader;
	}
		
	IShader* MakeShader(ShaderKey aKey)
	{
		IShader* spshader = nullptr;
		switch (aKey)
		{
		case ShaderKey::BasicShader:
			spshader = new BasicShader();
			break;
		case ShaderKey::PbrShader:
			spshader = new PbrShader();
			break;
		default:
			break;
		}

		if (spshader == nullptr)
		{
			throw std::runtime_error("MakeShader failed to make a new shader");
		}
		return spshader;
	}
};
