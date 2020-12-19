#include "Model.h"
#include "Graphics.h"
#include "Release.h"

std::unique_ptr<Model> Model::NewTriangle()
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

std::unique_ptr<Model> Model::NewQuad()
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

std::unique_ptr<Model> Model::NewCube()
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

Model::~Model()
{
	Common::Release<VertexBuffer>(pVBuffer);
	Common::Release<IndexBuffer>(pIBuffer);
}

void Model::Init(Graphics& arGraphics)
{
	pVBuffer = arGraphics.CreateVertexBuffer(Verts);
	pIBuffer = arGraphics.CreateIndexBuffer(Indices);
}

void Model::Update(Graphics& arGraphics)
{
	auto& rctx = arGraphics.GetDeviceCtx();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	rctx.IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
	rctx.IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);
	rctx.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


