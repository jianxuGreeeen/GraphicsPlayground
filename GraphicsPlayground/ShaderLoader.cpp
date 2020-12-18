#include "ShaderLoader.h"
#include "ShaderHelper.h"
#include "ShaderLayouts.h"
#include "ShaderNames.h"
#include "MathTypes.h"
#include <stdexcept>

namespace
{
	std::wstring MakeShaderFileName(const std::wstring& arShaderName)
	{
		return (arShaderName + L".cso");
	}
}

namespace VShaderLoader
{
	std::unique_ptr<VShaderInfo> LoadBasicShader(GraphicsDevice& arDevice)
	{
		auto filename = MakeShaderFileName(ShaderNames::BasicVertShader);
		auto* pcso = ShaderHelper::CreateCompiledShaderObject(filename);
		VertexShader* pvs = nullptr;

		auto hr = arDevice.CreateVertexShader(pcso->GetBufferPointer(), pcso->GetBufferSize(), nullptr, &pvs);
		if (FAILED(hr))
		{
			pcso->Release();
			throw new std::runtime_error("LoadBasicVShader failed to CreateVertexShader");
		}

		ShaderInputLayout* pvlayout = nullptr;
		auto vlayout = ShaderLayouts::GetBasicLayout();
		hr = arDevice.CreateInputLayout(vlayout.data(), static_cast<UINT>(vlayout.size()),
			pcso->GetBufferPointer(), pcso->GetBufferSize(), &pvlayout);

		pcso->Release();
		if (FAILED(hr))
		{
			pvs->Release();
			throw new std::runtime_error("LoadBasicVShader failed to CreateInputLayout");
		}

		return std::make_unique<VShaderInfo>(*pvs, *pvlayout);
	}
};

namespace PShaderLoader
{
	std::unique_ptr<PShaderInfo> LoadBasicShader(GraphicsDevice& arDevice)
	{
		auto filename = MakeShaderFileName(ShaderNames::BasicPixelShader);
		auto* pcso = ShaderHelper::CreateCompiledShaderObject(filename);
		PixelShader* pps = nullptr;
		auto hr = arDevice.CreatePixelShader(pcso->GetBufferPointer(), pcso->GetBufferSize(), nullptr, &pps);
		pcso->Release();

		if (FAILED(hr))
		{
			throw new std::runtime_error("LoadBasicPShader failed to CreatePixelShader");
		}
		return std::make_unique<PShaderInfo>(*pps);
	}
};
