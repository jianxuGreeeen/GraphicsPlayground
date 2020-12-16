#include "ShaderManager.h"
#include "ShaderHelper.h"
#include "ShaderLayouts.h"
#include <stdexcept>

void ShaderManager::Load(GraphicsDevice& arDevice, const std::vector<ShaderName>& arPixelShaders, const std::vector<ShaderName>& arVertexShaders)
{
	for (const auto& rshaderName : arVertexShaders)
	{
		auto filename = ShaderHelper::MakeShaderFileName(rshaderName);
		auto* pcso = ShaderHelper::CreateCompiledShaderObject(filename);
		VertexShader* pvs = nullptr;

		auto hr = arDevice.CreateVertexShader(pcso->GetBufferPointer(), pcso->GetBufferSize(), nullptr, &pvs);
		if (FAILED(hr))
		{
			pcso->Release();
			throw new std::runtime_error("ShaderManager failed to CreateVertexShader");
		}
		
		VertexLayout* pvlayout = nullptr;
		auto vlayout = ShaderLayouts::GetBasicLayout();
		hr = arDevice.CreateInputLayout(vlayout.data(), static_cast<UINT>(vlayout.size()), 
			pcso->GetBufferPointer(), pcso->GetBufferSize(), &pvlayout);

		pcso->Release();
		if (FAILED(hr))
		{
			throw new std::runtime_error("ShaderManager failed to CreateInputLayout");
		}

		VsTable.emplace(std::make_pair(rshaderName, std::make_unique<VertexShaderData>(pvs, pvlayout)));
	}

	for (const auto& rshaderName : arPixelShaders)
	{
		auto filename = ShaderHelper::MakeShaderFileName(rshaderName);
		auto* pcso = ShaderHelper::CreateCompiledShaderObject(filename);
		PixelShader* pps = nullptr;
		auto hr = arDevice.CreatePixelShader(pcso->GetBufferPointer(), pcso->GetBufferSize(), nullptr, &pps);
		pcso->Release();

		if (FAILED(hr))
		{
			throw new std::runtime_error("ShaderManager failed to CreatePixelShader");
		}
		PsTable.emplace(std::make_pair(rshaderName, pps));
	}
}

PixelShader& ShaderManager::GetPs(const ShaderName& arKey)
{
	return *(PsTable[arKey]);
}

VertexShaderData& ShaderManager::GetVs(const ShaderName& arKey)
{
	return *(VsTable[arKey]);
}

