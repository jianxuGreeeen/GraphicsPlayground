#pragma once
#include "GraphicsInterfaceObject.h"
#include "GraphicsTypes.h"
#include "ShaderNames.h"
#include "Vertex.h"
#include <vector>
#include <memory>

using ModelId = uint64_t;

class Model final
{
public:

	Model(const ShaderName& aPixelShader, const ShaderName& aVertexShader, std::vector<Vertex> arVerts)
		:Id(++sId)
		, PShader(aPixelShader)
		, VShader(aVertexShader)
		, Verts(std::move(arVerts))
	{}

	void Init(GraphicsDevice& arDevice);

	Model() = default;
	~Model() = default;

	const std::vector<Vertex>& GetVerts() const { return Verts; }
	int GetBufferSize() const { return sizeof(Verts);}

	const ShaderName& GetPShader() const { return PShader; }
	const ShaderName& GetVShader() const { return VShader; }
	VertexBuffer* const  GetVBuffer() { return spVBuffer.Get(); }
	VertexLayout* const GetVLayout() { return spVLayout.Get(); }

	static std::unique_ptr<Model> CreateTriangle();

private:

	static ModelId sId;

	ModelId Id = 0;
	ShaderName PShader;
	ShaderName VShader;
	std::vector<Vertex> Verts;
	GraphicsInterfaceObject<VertexBuffer> spVBuffer;
	GraphicsInterfaceObject<VertexLayout> spVLayout;
};

