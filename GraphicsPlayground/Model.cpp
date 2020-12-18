#include "Model.h"
#include <stdexcept>

ModelId Model::sId = 0;

std::unique_ptr<Model> Model::CreateTriangle()
{
	std::vector<Vertex> verts =
	{
		{ Float3(0.0f, 0.5f, 0), Color4(1.0f, 0.0f, 0.0f, 1.0) },
		{ Float3(0.5f, -0.5f, 0), Color4(0.0f, 1.0f, 0.0f, 1.0) },
		{ Float3(-0.5f, -0.5f, 0), Color4(0.0f, 0.0f, 1.0f, 1.0) },
	};

	std::vector<Index> indices =
	{
		{ 0, 1, 2 },
	};

	return std::make_unique<Model>(std::move(verts), std::move(indices));
}

std::unique_ptr<Model> Model::CreateQuad()
{
	std::vector<Vertex> verts =
	{
		{ Float3(-0.5f, -0.5f, 0), Color4(1.0f, 0.0f, 0.0f, 1.0) },
		{ Float3(-0.5f,  0.5f, 0), Color4(0.0f, 1.0f, 0.0f, 1.0) },
		{ Float3(0.5f,  0.5f, 0), Color4(0.0f, 0.0f, 1.0f, 1.0) },
		{ Float3(0.5f, -0.5f, 0), Color4(0.0f, 1.0f, 0.0f, 1.0) },
	};

	std::vector<Index> indices =
	{
		0, 1, 2,
		0, 2, 3,
	};

	return std::make_unique<Model>(std::move(verts), std::move(indices));
}

std::unique_ptr<Model> Model::CreateCube()
{
	std::vector<Vertex> verts =
	{
		{ Float3(-0.5f, -0.5f, -0.5f), Color4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ Float3(-0.5f, +0.5f, -0.5f), Color4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ Float3(+0.5f, +0.5f, -0.5f), Color4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ Float3(+0.5f, -0.5f, -0.5f), Color4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ Float3(-0.5f, -0.5f, +0.5f), Color4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ Float3(-0.5f, +0.5f, +0.5f), Color4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ Float3(+0.5f, +0.5f, +0.5f), Color4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ Float3(+0.5f, -0.5f, +0.5f), Color4(1.0f, 0.0f, 0.0f, 1.0f) },
	};

	std::vector<Index> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		//// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		0, 3, 4,
		4, 3, 7
	};

	return std::make_unique<Model>(std::move(verts), std::move(indices));
}

void Model::Init(GraphicsDevice& arDevice)
{
	CreateVertexBuffer(arDevice);
	CreateIndexBuffer(arDevice);
}

void Model::CreateVertexBuffer(GraphicsDevice& arDevice)
{
	GraphicsBufferDesc bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = NumVerts() * Vertex::ByteWidth;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	GraphicsBufferData bufferData = {};
	bufferData.pSysMem = Verts.data();

	spVBuffer = CreateBuffer(bufferDesc, bufferData, arDevice);
}

void Model::CreateIndexBuffer(GraphicsDevice& arDevice)
{
	GraphicsBufferDesc bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Index) * NumIndices(); //NumTriangles() * Vertex::ByteWidth;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	GraphicsBufferData bufferData = {};
	bufferData.pSysMem = Indices.data();

	spIBuffer = CreateBuffer(bufferDesc, bufferData, arDevice);
}

GraphicsBuffer* Model::CreateBuffer(const GraphicsBufferDesc& arDesc, const GraphicsBufferData& arData, GraphicsDevice& arDevice)
{
	GraphicsBuffer* pbuffer = nullptr;
	auto hr = arDevice.CreateBuffer(&arDesc, &arData, &pbuffer);
	if (FAILED(hr))
	{
		throw new std::runtime_error("CreateBuffer failed");
	}
	return pbuffer;
}


