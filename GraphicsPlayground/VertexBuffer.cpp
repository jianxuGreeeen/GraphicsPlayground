#include "VertexBuffer.h"
#include "Model.h"
#include "d3dx12.h"
#include <stdexcept>

void VertexBuffer::Init(GraphicsDevice& arDevice, const Model& arModel)
{
    const auto& rvlist = arModel.GetVerts();
    const int size = sizeof(rvlist);

    GraphicsBuffer* pbuffer = nullptr;
    auto hr = arDevice.CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), // a default heap
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(size),
        D3D12_RESOURCE_STATE_COPY_DEST, 
        nullptr,
        IID_PPV_ARGS(&pbuffer));

    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to init vertex buffer heap");
    }
    else
    {
        spBuffer = pbuffer;
        spBuffer->SetName(L"Vertex Buffer Resource Heap");
    }

    hr = arDevice.CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(size),
        D3D12_RESOURCE_STATE_GENERIC_READ, 
        nullptr,
        IID_PPV_ARGS(&pbuffer));
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to init vertex buffer upload heap");
    }
    else
    {
        spUploadBuffer = pbuffer;
        spUploadBuffer->SetName(L"Vertex Buffer Upload Resource Heap");
    }
}
