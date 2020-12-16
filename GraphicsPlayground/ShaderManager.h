#pragma once
#include "GraphicsInterfaceObject.h"
#include "GraphicsTypes.h"
#include "ShaderNames.h"
#include <map>
#include <memory>
#include <vector>

struct VertexShaderData
{
	VertexShaderData(VertexShader* apVs, VertexLayout* apVLayout)
		: Vs(apVs), VsLayout(apVLayout)
	{
	}

	~VertexShaderData()
	{
		Vs.Release();
		VsLayout.Release();
	}

	GraphicsInterfaceObject<VertexShader> Vs;
	GraphicsInterfaceObject<VertexLayout> VsLayout;
};

//TODO: I think we need an additional layer to store combinations of layout, vshader, pshader
class ShaderManager final
{
public:
	ShaderManager() = default;
	~ShaderManager() = default;

	void Load(GraphicsDevice& arDevice, const std::vector<ShaderName>& arPixelShaders, const std::vector<ShaderName>& arVertexShaders);

	PixelShader& GetPs(const ShaderName& arKey);
	VertexShaderData& GetVs(const ShaderName& arKey);

private:
	ShaderManager(ShaderManager&) = delete;
	ShaderManager(ShaderManager&&) = delete;
	ShaderManager& operator = (ShaderManager&) = delete;
	ShaderManager& operator = (ShaderManager&&) = delete;

	std::map<ShaderName, std::unique_ptr<VertexShaderData>> VsTable;
	std::map<ShaderName, GraphicsInterfaceObject<PixelShader>> PsTable;
};

