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

std::unique_ptr<Model> Model::NewSphere(size_t aSubDivision)
{
	//https://schneide.blog/2016/07/15/generating-an-icosphere-in-c/
	//Icosphere
	constexpr float X = .525731112119133606f;
	constexpr float Z = .850650808352039932f;
	constexpr float N = 0.f;

	using VertList = std::vector<Vertex>;
	VertList verts =
	{
	  {Float3(-X,N,Z), Color4(1.0f, 0.0f, 0.0f, 1.0f)},
	  {Float3(X,N,Z), Color4(0.0f, 1.0f, 0.0f, 1.0f)},
	  {Float3(-X,N,-Z), Color4(0.0f, 0.0f, 1.0f, 1.0f)},
	  {Float3(X,N,-Z), Color4(1.0f, 1.0f, 0.0f, 1.0f)},
	  {Float3(N,Z,X), Color4(0.0f, 1.0f, 1.0f, 1.0f)},
	  {Float3(N,Z,-X), Color4(1.0f, 1.0f, 1.0f, 1.0f)},
	  {Float3(N,-Z,X), Color4(1.0f, 0.0f, 1.0f, 1.0f)},
	  {Float3(N,-Z,-X), Color4(1.0f, 0.0f, 0.0f, 1.0f)},
	  {Float3(Z,X,N), Color4(1.0f, 0.0f, 0.0f, 1.0f)},
	  {Float3(-Z,X, N), Color4(0.0f, 1.0f, 0.0f, 1.0f)},
	  {Float3(Z,-X,N), Color4(0.0f, 0.0f, 1.0f, 1.0f)},
	  {Float3(-Z,-X, N), Color4(1.0f, 1.0f, 0.0f, 1.0f)}
	};

	using IndexList = std::vector<Index>;
	IndexList indices =
	{
		0,4,1,	0,9,4,	9,5,4,	4,5,8,
		4,8,1,	8,10,1, 8,3,10, 5,3,8,
		5,2,3,	2,7,3,	7,10,3, 7,6,10,
		7,11,6, 11,0,6, 0,1,6,	6,1,10,
		9,0,11, 9,11,2, 9,2,5,	7,2,11
	};

	//Subdivide
	using namespace DirectX;
	using Lookup = std::map<std::pair<Index, Index>, Index>;
	auto vertex_for_edge = [&](Lookup& lookup,
		VertList& arVerts, Index first, Index second)
	{
		Lookup::key_type key(first, second);
		if (key.first > key.second)
		{
			std::swap(key.first, key.second);
		}

		auto inserted = lookup.insert({ key, static_cast<Index>(arVerts.size())});
		if (inserted.second)
		{
			auto& edge0 = arVerts[first];
			auto& edge1 = arVerts[second];
			Float3 middle = {
				(edge0.Pos.x + edge1.Pos.x) / 2.0f,
				(edge0.Pos.y + edge1.Pos.y) / 2.0f,
				(edge0.Pos.z + edge1.Pos.z) / 2.0f };

			auto v = XMLoadFloat3(&middle);
			auto normalizedV = DirectX::XMVector3Normalize(v);
			Vertex newVert;
			XMStoreFloat3(&(newVert.Pos), normalizedV);
			arVerts.push_back(newVert);
		}
		return inserted.first->second;
	};

	auto subdivide = [&](VertList& arVerts, IndexList aIndices)
	{
		Lookup lookup;
		IndexList result;

		for (size_t j = 0u; j < aIndices.size(); j+=3)
		{
			const std::array<Index, 3> triangleIndices =
			{
				aIndices[j + 0], aIndices[j + 1], aIndices[j + 2]
			};

			std::array<Index, 3> mid;
			for (size_t edge = 0u; edge < 3u; ++edge)
			{
				mid[edge] = vertex_for_edge(lookup, arVerts,
					triangleIndices[edge], triangleIndices[(edge + 1u) % 3u]);
			}

			IndexList newIndices =
			{
				triangleIndices[0], mid[0], mid[2],
				triangleIndices[1], mid[1], mid[0],
				triangleIndices[2], mid[2], mid[1],
				mid[0], mid[1], mid[2]
			};
			 
			result.insert(std::end(result), std::begin(newIndices), std::end(newIndices));
		}

		return result;
	};

	for (int i = 0; i < aSubDivision; ++i)
	{
		indices = subdivide(verts, indices);
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


