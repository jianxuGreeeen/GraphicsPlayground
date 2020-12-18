#include "Model.h"
#include <stdexcept>

ModelId Model::sId = 0;

std::unique_ptr<Model> Model::CreateTriangle(float aPosZ)
{
	std::vector<Vertex> verts =
	{
		{ Vector3(0.0f, 0.5f, aPosZ), Color4(1.0f, 0.0f, 0.0f, 1.0) },
		{ Vector3(0.5f, -0.5f, aPosZ), Color4(0.0f, 1.0f, 0.0f, 1.0) },
		{ Vector3 (-0.5f, -0.5f, aPosZ), Color4(0.0f, 0.0f, 1.0f, 1.0) },
	};

	std::vector<Index> indices =
	{
		{ 0, 1, 2 },
	};

	return std::make_unique<Model>(std::move(verts), std::move(indices));
}

std::unique_ptr<Model> Model::CreateQuad(float aPosZ)
{
	std::vector<Vertex> verts =
	{
		{ Vector3(-0.5f, -0.5f, aPosZ), Color4(1.0f, 0.0f, 0.0f, 1.0) },
		{ Vector3(-0.5f,  0.5f, aPosZ), Color4(0.0f, 1.0f, 0.0f, 1.0) },
		{ Vector3(0.5f,  0.5f, aPosZ), Color4(0.0f, 0.0f, 1.0f, 1.0) },
		{ Vector3(0.5f, -0.5f, aPosZ), Color4(0.0f, 1.0f, 0.0f, 1.0) },
	};

	std::vector<Index> indices =
	{
		0, 1, 2,
		0, 2, 3,
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
	bufferDesc.ByteWidth = sizeof(Index) * NumTriangles() * Vertex::ByteWidth;
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


