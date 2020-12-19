#include "ShaderHelper.h"
#include "BasicShader.h"

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
		
	std::unique_ptr<IShader> MakeShader(ShaderKey aKey)
	{
		std::unique_ptr<IShader> spshader = nullptr;
		switch (aKey)
		{
		case ShaderKey::BasicShader:
			spshader = std::make_unique<BasicShader>();
			break;
		default:
			break;
		}

		if (spshader == nullptr)
		{
			throw std::runtime_error("MakeBasicShader failed to make a new shader");
		}
		return spshader;
	}
};
