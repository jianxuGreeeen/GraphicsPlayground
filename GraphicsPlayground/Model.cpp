#include "Model.h"
#include <stdexcept>

ModelId Model::sId = 0;

std::unique_ptr<Model> Model::CreateTriangle()
{
	std::vector<Vertex> verts =
	{
		{ 0.0f, 0.5f, 0.5f },
		{ 0.5f, -0.5f, 0.5f },
		{ -0.5f, -0.5f, 0.5f },
	};

	return std::make_unique<Model>(ShaderNames::BasicPixelShader, ShaderNames::BasicVertShader, std::move(verts));
}

void Model::Init(GraphicsDevice& arDevice)
{
	D3D11_BUFFER_DESC bufferDesc = {};

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = GetBufferSize() * 7;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA bufferData = {};
	bufferData.pSysMem = Verts.data();

	VertexBuffer* pbuffer = nullptr;
	auto hr = arDevice.CreateBuffer(&bufferDesc, &bufferData, &pbuffer);
	if (FAILED(hr))
	{
		throw new std::runtime_error("Model failed to create Vertex Buffer");
	}
	
	spVBuffer = pbuffer;
}
