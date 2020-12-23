#include "Model.h"
#include "Graphics.h"
#include "Release.h"
#include <math.h>

std::unique_ptr<Model> Model::NewTriangle()
{
	std::vector<Vertex> verts =
	{
		{ Float3(0.0f, 0.5f, 0), Color4(1.0f, 0.0f, 0.0f, 1.0), Float2(1.0, 0.0f), Float3(0.0, 0.0f, -1.0f) },
		{ Float3(0.5f, -0.5f, 0), Color4(0.0f, 1.0f, 0.0f, 1.0), Float2(1.0, 1.0f), Float3(0.0, 0.0f, -1.0f) },
		{ Float3(-0.5f, -0.5f, 0), Color4(0.0f, 0.0f, 1.0f, 1.0), Float2(0.0f, 1.0f), Float3(0.0, 0.0f, -1.0f)},
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
		{ Float3(-0.5f, -0.5f, 0), Color4(1.0f, 0.0f, 0.0f, 1.0), Float2(0.0f, 1.0f), Float3(0.0, 0.0f, -1.0f) },
		{ Float3(-0.5f,  0.5f, 0), Color4(0.0f, 1.0f, 0.0f, 1.0), Float2(0.0f, 0.0f), Float3(0.0, 0.0f, -1.0f) },
		{ Float3(0.5f,  0.5f, 0), Color4(0.0f, 0.0f, 1.0f, 1.0), Float2(1.0f, 0.0f), Float3(0.0, 0.0f, -1.0f) },
		{ Float3(0.5f, -0.5f, 0), Color4(0.0f, 1.0f, 0.0f, 1.0), Float2(1.0f, 1.0f), Float3(0.0, 0.0f, -1.0f) },
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
		// front
		{ Float3(-0.5f, -0.5f, -0.5f), Color4(1.0f, 0.0f, 0.0f, 1.0f), Float2(0.0f, 1.0f), Float3(-1.0, -1.0f, -1.0f) },
		{ Float3(-0.5f, +0.5f, -0.5f), Color4(0.0f, 1.0f, 0.0f, 1.0f), Float2(0.0f, 0.0f), Float3(-1.0, +1.0f, -1.0f)  },
		{ Float3(+0.5f, +0.5f, -0.5f), Color4(0.0f, 0.0f, 1.0f, 1.0f), Float2(1.0f, 0.0f), Float3(+1.0, +1.0f, -1.0f)  },
		{ Float3(+0.5f, -0.5f, -0.5f), Color4(1.0f, 1.0f, 0.0f, 1.0f), Float2(1.0f, 1.0f), Float3(+1.0, -1.0f, -1.0f)  },
		// back
		{ Float3(-0.5f, -0.5f, +0.5f), Color4(0.0f, 1.0f, 1.0f, 1.0f), Float2(1.0f, 1.0f), Float3(-1.0, -1.0f, +1.0f)  },
		{ Float3(+0.5f, -0.5f, +0.5f), Color4(1.0f, 1.0f, 1.0f, 1.0f), Float2(0.0f, 1.0f), Float3(+1.0, -1.0f, +1.0f)  },
		{ Float3(+0.5f, +0.5f, +0.5f), Color4(1.0f, 0.0f, 1.0f, 1.0f), Float2(0.0f, 0.0f), Float3(+1.0, +1.0f, +1.0f)  },
		{ Float3(-0.5f, +0.5f, +0.5f), Color4(1.0f, 0.0f, 0.0f, 1.0f), Float2(1.0f, 0.0f), Float3(-1.0, +1.0f, +1.0f)  },
		// top
		{ Float3(-0.5f, +0.5f, -0.5f), Color4(1.0f, 0.0f, 0.0f, 1.0f), Float2(0.0f, 1.0f), Float3(-1.0, +1.0f, -1.0f)  },
		{ Float3(-0.5f, +0.5f, +0.5f), Color4(0.0f, 1.0f, 0.0f, 1.0f), Float2(0.0f, 0.0f), Float3(-1.0, +1.0f, +1.0f)  },
		{ Float3(+0.5f, +0.5f, +0.5f), Color4(0.0f, 0.0f, 1.0f, 1.0f), Float2(1.0f, 0.0f), Float3(+1.0, +1.0f, +1.0f)  },
		{ Float3(+0.5f, +0.5f, -0.5f), Color4(1.0f, 1.0f, 0.0f, 1.0f), Float2(1.0f, 1.0f), Float3(+1.0, +1.0f, -1.0f)  },
		// bottom
		{ Float3(-0.5f, -0.5f, -0.5f), Color4(0.0f, 1.0f, 1.0f, 1.0f), Float2(1.0f, 1.0f), Float3(-1.0, -1.0f, -1.0f)  },
		{ Float3(+0.5f, -0.5f, -0.5f), Color4(1.0f, 1.0f, 1.0f, 1.0f), Float2(0.0f, 1.0f), Float3(+1.0, -1.0f, -1.0f)  },
		{ Float3(+0.5f, -0.5f, +0.5f), Color4(1.0f, 0.0f, 1.0f, 1.0f), Float2(0.0f, 0.0f), Float3(+1.0, -1.0f, +1.0f)  },
		{ Float3(-0.5f, -0.5f, +0.5f), Color4(1.0f, 0.0f, 0.0f, 1.0f), Float2(1.0f, 0.0f), Float3(-1.0, -1.0f, +1.0f)  },
		// left
		{ Float3(-0.5f, -0.5f, +0.5f), Color4(0.0f, 1.0f, 1.0f, 1.0f), Float2(0.0f, 1.0f), Float3(-1.0, -1.0f, +1.0f)  },
		{ Float3(-0.5f, +0.5f, +0.5f), Color4(1.0f, 1.0f, 1.0f, 1.0f), Float2(0.0f, 0.0f), Float3(-1.0, +1.0f, +1.0f)  },
		{ Float3(-0.5f, +0.5f, -0.5f), Color4(1.0f, 0.0f, 1.0f, 1.0f), Float2(1.0f, 0.0f), Float3(-1.0, +1.0f, -1.0f)  },
		{ Float3(-0.5f, -0.5f, -0.5f), Color4(1.0f, 0.0f, 0.0f, 1.0f), Float2(1.0f, 1.0f), Float3(-1.0, -1.0f, -1.0f)  },
		// right
		{ Float3(+0.5f, -0.5f, -0.5f), Color4(0.0f, 1.0f, 1.0f, 1.0f), Float2(0.0f, 1.0f), Float3(+1.0, -1.0f, -1.0f)  },
		{ Float3(+0.5f, +0.5f, -0.5f), Color4(1.0f, 1.0f, 1.0f, 1.0f), Float2(0.0f, 0.0f), Float3(+1.0, +1.0f, -1.0f)  },
		{ Float3(+0.5f, +0.5f, +0.5f), Color4(1.0f, 0.0f, 1.0f, 1.0f), Float2(1.0f, 0.0f), Float3(+1.0, +1.0f, +1.0f)  },
		{ Float3(+0.5f, -0.5f, +0.5f), Color4(1.0f, 0.0f, 0.0f, 1.0f), Float2(1.0f, 1.0f), Float3(+1.0, -1.0f, +1.0f)  },
	};

	std::vector<Index> indices =
	{
		// front
		0,  1,  2,
		0,  2,  3,
		// back
		4,  5,  6,
		4,  6,  7,
		// top
		8,  9, 10,
		8, 10, 11,
		// bottom
		12, 13, 14,
		12, 14, 15,
		// left
		16, 17, 18,
		16, 18, 19,
		// right
		20, 21, 22,
		20, 22, 23
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


