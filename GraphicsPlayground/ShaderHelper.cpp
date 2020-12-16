#include "ShaderHelper.h"
#include <d3dcompiler.h>
#include <stdexcept>

namespace ShaderHelper
{
	CompiledShaderObject* CreateCompiledShaderObject(const std::wstring& arFileName)
	{
		CompiledShaderObject* pshader = nullptr; // d3d blob for holding vertex shader bytecode
		auto hr = D3DReadFileToBlob(arFileName.c_str(), &pshader);
		if (FAILED(hr))
		{
			throw std::runtime_error("CompiledShaderObject failed to read from file");
		}

		return pshader;
	}

	std::wstring MakeShaderFileName(const std::wstring& arShaderName)
	{
		return (arShaderName + L".cso");
	}
};
