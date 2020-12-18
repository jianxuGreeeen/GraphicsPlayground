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

	Model(std::vector<Vertex> aVerts, std::vector<Index> aIndices)
		: Id(++sId)
		, Verts(std::move(aVerts))
		, Indices((aIndices))
	{}

	void Init(GraphicsDevice& arDevice);

	Model() : Id(++sId) {};
	~Model() = default;

	const ModelId GetId() const { return sId; }
	const PShaderKey GetPShader() const { return ShaderInfo.PShader; }
	const VShaderKey GetVShader() const { return ShaderInfo.VShader; }
	VertexBuffer* const  GetVBuffer() { return spVBuffer.Get(); }
	IndexBuffer* const  GetIBuffer() { return spIBuffer.Get(); }

	int NumIndices() const { return static_cast<int>(Indices.size()); }

	static std::unique_ptr<Model> CreateTriangle();
	static std::unique_ptr<Model> CreateQuad();
	static std::unique_ptr<Model> CreateCube();

private:

	int NumVerts() const { return static_cast<int>(Verts.size()); }
	int NumTriangles() const { return NumIndices() / 3; }

	static ModelId sId;

	ModelId Id = 0;
	ModelShaderInfo ShaderInfo;
	std::vector<Vertex> Verts;
	std::vector<Index> Indices;
	GraphicsInterfaceObject<VertexBuffer> spVBuffer;
	GraphicsInterfaceObject<IndexBuffer> spIBuffer;

	void CreateVertexBuffer(GraphicsDevice& arDevice);
	void CreateIndexBuffer(GraphicsDevice& arDevice);

	static GraphicsBuffer* CreateBuffer(const GraphicsBufferDesc& arDesc, const GraphicsBufferData& arData, GraphicsDevice& arDevice);
};

