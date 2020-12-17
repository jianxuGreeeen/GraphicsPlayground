#pragma once
#include "GraphicsInterfaceObject.h"
#include "GraphicsTypes.h"
#include "ShaderKey.h"
#include "Vertex.h"
#include <vector>
#include <memory>

using ModelId = uint64_t;

struct ModelShaderInfo final
{
	PShaderKey PShader = PShaderKey::BasicShader;
	VShaderKey VShader = VShaderKey::BasicShader;
};

class Model final
{
public:

	Model(std::vector<Vertex> aVerts)
		:Id(++sId)
		, Verts(std::move(aVerts))
	{}

	void Init(GraphicsDevice& arDevice);

	Model() : Id(++sId) {};
	~Model() = default;

	const ModelId GetId() const { return sId; }
	const std::vector<Vertex>& GetVerts() const { return Verts; }
	int GetBufferSize() const { return sizeof(Verts);}

	const PShaderKey GetPShader() const { return ShaderInfo.PShader; }
	const VShaderKey GetVShader() const { return ShaderInfo.VShader; }
	VertexBuffer* const  GetVBuffer() { return spVBuffer.Get(); }

	static std::unique_ptr<Model> CreateTriangle();

private:

	static ModelId sId;

	ModelId Id = 0;
	ModelShaderInfo ShaderInfo;
	std::vector<Vertex> Verts;
	GraphicsInterfaceObject<VertexBuffer> spVBuffer;
};

