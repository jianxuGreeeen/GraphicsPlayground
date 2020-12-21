#include "Model.h"
#include "Graphics.h"
#include "Release.h"
#include <math.h>

std::unique_ptr<Model> Model::NewTriangle()
{
	std::vector<Vertex> verts =
	{
		{ Float3(0.0f, 0.5f, 0), Color4(1.0f, 0.0f, 0.0f, 1.0), Float2(1.0, 0.0f) },
		{ Float3(0.5f, -0.5f, 0), Color4(0.0f, 1.0f, 0.0f, 1.0), Float2(1.0, 1.0f) },
		{ Float3(-0.5f, -0.5f, 0), Color4(0.0f, 0.0f, 1.0f, 1.0), Float2(0.0f, 1.0f)},
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
		{ Float3(-0.5f, -0.5f, 0), Color4(1.0f, 0.0f, 0.0f, 1.0), Float2(0.0f, 1.0f) },
		{ Float3(-0.5f,  0.5f, 0), Color4(0.0f, 1.0f, 0.0f, 1.0), Float2(0.0f, 0.0f) },
		{ Float3(0.5f,  0.5f, 0), Color4(0.0f, 0.0f, 1.0f, 1.0), Float2(1.0f, 0.0f) },
		{ Float3(0.5f, -0.5f, 0), Color4(0.0f, 1.0f, 0.0f, 1.0), Float2(1.0f, 1.0f) },
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
		{ Float3(-0.5f, -0.5f, -0.5f), Color4(1.0f, 0.0f, 0.0f, 1.0f), Float2(0.0f, 1.0f) },
		{ Float3(-0.5f, +0.5f, -0.5f), Color4(0.0f, 1.0f, 0.0f, 1.0f), Float2(0.0f, 0.0f) },
		{ Float3(+0.5f, +0.5f, -0.5f), Color4(0.0f, 0.0f, 1.0f, 1.0f), Float2(1.0f, 0.0f) },
		{ Float3(+0.5f, -0.5f, -0.5f), Color4(1.0f, 1.0f, 0.0f, 1.0f), Float2(1.0f, 1.0f) },
		// back
		{ Float3(-0.5f, -0.5f, +0.5f), Color4(0.0f, 1.0f, 1.0f, 1.0f), Float2(1.0f, 1.0f) },
		{ Float3(+0.5f, -0.5f, +0.5f), Color4(1.0f, 1.0f, 1.0f, 1.0f), Float2(0.0f, 1.0f) },
		{ Float3(+0.5f, +0.5f, +0.5f), Color4(1.0f, 0.0f, 1.0f, 1.0f), Float2(0.0f, 0.0f) },
		{ Float3(-0.5f, +0.5f, +0.5f), Color4(1.0f, 0.0f, 0.0f, 1.0f), Float2(1.0f, 0.0f) },
		// top
		{ Float3(-0.5f, +0.5f, -0.5f), Color4(1.0f, 0.0f, 0.0f, 1.0f), Float2(0.0f, 1.0f) },
		{ Float3(-0.5f, +0.5f, +0.5f), Color4(0.0f, 1.0f, 0.0f, 1.0f), Float2(0.0f, 0.0f) },
		{ Float3(+0.5f, +0.5f, +0.5f), Color4(0.0f, 0.0f, 1.0f, 1.0f), Float2(1.0f, 0.0f) },
		{ Float3(+0.5f, +0.5f, -0.5f), Color4(1.0f, 1.0f, 0.0f, 1.0f), Float2(1.0f, 1.0f) },
		// bottom
		{ Float3(-0.5f, -0.5f, -0.5f), Color4(0.0f, 1.0f, 1.0f, 1.0f), Float2(1.0f, 1.0f) },
		{ Float3(+0.5f, -0.5f, -0.5f), Color4(1.0f, 1.0f, 1.0f, 1.0f), Float2(0.0f, 1.0f) },
		{ Float3(+0.5f, -0.5f, +0.5f), Color4(1.0f, 0.0f, 1.0f, 1.0f), Float2(0.0f, 0.0f) },
		{ Float3(-0.5f, -0.5f, +0.5f), Color4(1.0f, 0.0f, 0.0f, 1.0f), Float2(1.0f, 0.0f) },
		// left
		{ Float3(-0.5f, -0.5f, +0.5f), Color4(0.0f, 1.0f, 1.0f, 1.0f), Float2(0.0f, 1.0f) },
		{ Float3(-0.5f, +0.5f, +0.5f), Color4(1.0f, 1.0f, 1.0f, 1.0f), Float2(0.0f, 0.0f) },
		{ Float3(-0.5f, +0.5f, -0.5f), Color4(1.0f, 0.0f, 1.0f, 1.0f), Float2(1.0f, 0.0f) },
		{ Float3(-0.5f, -0.5f, -0.5f), Color4(1.0f, 0.0f, 0.0f, 1.0f), Float2(1.0f, 1.0f) },
		// right
		{ Float3(+0.5f, -0.5f, -0.5f), Color4(0.0f, 1.0f, 1.0f, 1.0f), Float2(0.0f, 1.0f) },
		{ Float3(+0.5f, +0.5f, -0.5f), Color4(1.0f, 1.0f, 1.0f, 1.0f), Float2(0.0f, 0.0f) },
		{ Float3(+0.5f, +0.5f, +0.5f), Color4(1.0f, 0.0f, 1.0f, 1.0f), Float2(1.0f, 0.0f) },
		{ Float3(+0.5f, -0.5f, +0.5f), Color4(1.0f, 0.0f, 0.0f, 1.0f), Float2(1.0f, 1.0f) },
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

std::unique_ptr<Model> Model::NewSphere(int aSubDivision)
{
	//https://www.braynzarsoft.net/viewtutorial/q16390-20-cube-mapping-skybox
	using namespace DirectX;

	const int parallels = aSubDivision;
	const int meridians = aSubDivision;
	const int numSphereVertices = ((parallels - 2) * meridians) + 2;
	const int numSphereFaces = ((parallels - 3) * (meridians) * 2) + (meridians * 2);

	std::vector<Vertex> verts(numSphereVertices);
	std::vector<Index> indices(numSphereFaces*3);

	verts[0].Pos = { 0.0f, 1.0f, 0.0f };
	for (int i = 0; i < parallels - 2; ++i)
	{
		const float pitch = static_cast<float>(i + 1) * (XM_PI/ (parallels - 1));
		const auto rotx = XMMatrixRotationX(pitch);

		for (int j = 0; j < meridians; ++j)
		{
			const float yaw = static_cast<float>(j) * (XM_2PI / meridians);
			const auto roty = XMMatrixRotationY(yaw);
			auto vertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), (rotx * roty));
			vertPos = XMVector3Normalize(vertPos);

			XMStoreFloat3(&(verts[i * meridians + j + 1].Pos), vertPos);
		}
	}
	verts[numSphereVertices - 1].Pos = { 0.0f, -1.0f, 0.0f };

	int k = 0;
	for (int l = 0; l < meridians - 1; ++l)
	{
		indices[k] = 0;
		indices[k + 1] = l + 1;
		indices[k + 2] = l + 2;
		k += 3;
	}

	indices[k] = 0;
	indices[k + 1] = meridians;
	indices[k + 2] = 1;
	k += 3;

	for (int i = 0; i < parallels - 3; ++i)
	{
		for (int j = 0; j < meridians - 1; ++j)
		{
			indices[k] = i * meridians + j + 1;
			indices[k + 1] = i * meridians + j + 2;
			indices[k + 2] = (i + 1) * meridians + j + 1;

			indices[k + 3] = (i + 1) * meridians + j + 1;
			indices[k + 4] = i * meridians + j + 2;
			indices[k + 5] = (i + 1) * meridians + j + 2;

			k += 6; // next quad
		}

		indices[k] = (i * meridians) + meridians;
		indices[k + 1] = (i * meridians) + 1;
		indices[k + 2] = ((i + 1) * meridians) + meridians;

		indices[k + 3] = ((i + 1) * meridians) + meridians;
		indices[k + 4] = (i * meridians) + 1;
		indices[k + 5] = ((i + 1) * meridians) + 1;

		k += 6;
	}

	for (int l = 0; l < meridians - 1; ++l)
	{
		indices[k] = numSphereVertices - 1;
		indices[k + 1] = (numSphereVertices - 1) - (l + 1);
		indices[k + 2] = (numSphereVertices - 1) - (l + 2);
		k += 3;
	}

	indices[k] = numSphereVertices - 1;
	indices[k + 1] = (numSphereVertices - 1) - meridians;
	indices[k + 2] = numSphereVertices - 2;

	for (auto& rvert : verts)
	{
		Vector posv = XMLoadFloat3(&rvert.Pos);
		posv = XMVector4Normalize(posv);

		Float3 normalizedPos;
		XMStoreFloat3(&normalizedPos, posv);

		const float atan2f = static_cast<float>(atan2(normalizedPos.x, normalizedPos.z));
		rvert.UV.x = (atan2f / XM_2PI) + 0.5f;
		rvert.UV.y = 1.0f - ((normalizedPos.y * 0.5f) + 0.5f);
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


