#pragma once
#include "GraphicsTypes.h"
#include "Vertex.h"
#include <memory>
#include <vector>

class Graphics;

class Model final
{
public:

	Model(std::vector<Vertex> aVerts, std::vector<Index> aIndices)
		: Verts(std::move(aVerts))
		, Indices((aIndices))
		, pVBuffer(nullptr)
		, pIBuffer(nullptr)
	{}

	void Init(Graphics& arGraphics);
	void Update(Graphics& arGraphics);

	Model() = default;
	~Model();

	int NumIndices() const { return static_cast<int>(Indices.size()); }

	static std::unique_ptr<Model> NewTriangle();
	static std::unique_ptr<Model> NewQuad();
	static std::unique_ptr<Model> NewCube();
	static std::unique_ptr<Model> NewSphere(int aSubDivision = 15);

private:

	int NumVerts() const { return static_cast<int>(Verts.size()); }
	int NumTriangles() const { return NumIndices() / 3; }

	std::vector<Vertex> Verts;
	std::vector<Index> Indices;
	VertexBuffer* pVBuffer = nullptr;
	IndexBuffer* pIBuffer = nullptr;
};

