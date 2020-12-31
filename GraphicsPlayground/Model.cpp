#include "Model.h"
#include "Graphics.h"
#include "MathTypes.h"
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


// https://github.com/JJJohan/PBR/blob/master/PBR/Shapes.cpp
std::unique_ptr<Model> Model::NewSphere()
{
	using namespace DirectX;

	constexpr int sliceCount = 20;
	constexpr int stackCount = 20;
	constexpr float radius = 0.5f;

	std::vector<Vertex> verts(sliceCount * stackCount + 1);
	std::vector<Index> indices(sliceCount * 6 + (stackCount - 2) * sliceCount * 6);

	verts[0].Pos = { 0.0f, radius, 0.0f };
	verts[0].Normal = { 0.0f, 1.0f, 0.0f };
	verts[0].UV = { 0.0f, 0.0f };

	const float phiStep = XM_PI / stackCount;
	const float thetaStep = XM_2PI / sliceCount;

	int vertexIndex = 1;
	for (int i = 1; i <= stackCount - 1; i++)
	{
		const float phi = i * phiStep;
		for (int j = 0; j <= sliceCount; j++)
		{
			const float theta = j * thetaStep;
			verts[vertexIndex].Pos = Float3(
				radius * XMScalarSin(phi) * XMScalarCos(theta),
				radius * XMScalarCos(phi),
				radius * XMScalarSin(phi) * XMScalarSin(theta));

			XMStoreFloat3(&verts[vertexIndex].Normal,
				XMVector3Normalize(XMLoadFloat3(&verts[vertexIndex].Pos)));

			verts[vertexIndex].UV = Float2(theta / XM_2PI, phi / XM_PI);

			++vertexIndex;
		}
	}

	verts[vertexIndex].Pos = { 0.0f, -radius, 0.0f };
	verts[vertexIndex].Normal = { 0.0f, -1.0f, 0.0f };
	verts[vertexIndex].UV = { 0.0f, 1.0f };

	++vertexIndex;

	int index = 0;
	for (int i = 1; i <= sliceCount; i++)
	{
		indices[index++] = 0;
		indices[index++] = i + 1;
		indices[index++] = i;
	}
	int baseIndex = 1;
	const int ringVertexCount = sliceCount + 1;
	for (int i = 0; i < stackCount - 2; i++)
	{
		for (int j = 0; j < sliceCount; j++)
		{
			indices[index++] = baseIndex + i * ringVertexCount + j;
			indices[index++] = baseIndex + i * ringVertexCount + j + 1;
			indices[index++] = baseIndex + (i + 1) * ringVertexCount + j;

			indices[index++] = baseIndex + (i + 1) * ringVertexCount + j;
			indices[index++] = baseIndex + i * ringVertexCount + j + 1;
			indices[index++] = baseIndex + (i + 1) * ringVertexCount + j + 1;
		}
	}
	const int southPoleIndex = vertexIndex - 1;
	baseIndex = southPoleIndex - ringVertexCount;
	for (int i = 0; i < sliceCount; i++)
	{
		indices[index++] = southPoleIndex;
		indices[index++] = baseIndex + i;
		indices[index++] = baseIndex + i + 1;
	}
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


