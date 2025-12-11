#include "Pyramid.h"


Pyramid::Pyramid(RenderData* render_in, const BodyType& type) : BaseHardObject(render_in, type)
{
    index_count_ = 18;
    InitialiseBuffers();
}

HRESULT Pyramid::InitialiseBuffers()
{
    HRESULT hr = S_OK;
    
    CreateVertexBuffer(hr);
    if (FAILED(hr)) return hr;
    //--------------------------------------------------//
    CreateIndexBuffer(hr);
    if (FAILED(hr)) return hr;
    
    return S_OK;
}

HRESULT Pyramid::CreateVertexBuffer(HRESULT& hr)
{
    SimpleVertex vertex_data[] = 
    {
        // Positions                                       // Tex Coords                   // Normals
        { XMFLOAT3(-1.00f,  0, 1),XMFLOAT3(-1.00f,  0, 1), XMFLOAT2(0.0f, 1.0f)},  // Front left
        
        { XMFLOAT3(1.00f,  0, 1),XMFLOAT3(1.00f,  0, 1), XMFLOAT2(1.0f, 1.0f)},   // Front right
       
        { XMFLOAT3(-1.00f, 0, -1),XMFLOAT3(-1.00f, 0, -1), XMFLOAT2(1.0f, 1.0f)},  // Back left
        
        { XMFLOAT3(1.00f, 0, -1),XMFLOAT3(1.00f, 0, -1), XMFLOAT2(0.0f, 1.0f)},   // Back right
        
        { XMFLOAT3(0,  2, 0),XMFLOAT3(0,  2, 0), XMFLOAT2(0.0f, 0.5f)}        // Top
    };
    //--------------------------------------------------//
    D3D11_BUFFER_DESC vb_desc = {};
    vb_desc.ByteWidth = sizeof(vertex_data);
    vb_desc.Usage = D3D11_USAGE_IMMUTABLE;
    vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA sub_vertex_data = { vertex_data };
    hr = render_->device_->CreateBuffer(&vb_desc, &sub_vertex_data, &vertex_buffer_);
    if (FAILED(hr)) { return hr; }
    //--------------------------------------------------//
    return hr;
}

HRESULT Pyramid::CreateIndexBuffer(HRESULT& hr)
{
    WORD index_data[] =
    {
        // Bottom
        0, 2, 1,
        1, 2, 3,
        // Front
        0, 1, 4,
        // Back
        2, 4, 3,
        // Right
        1, 3, 4,
        // Left
        0, 4, 2
    };
    //--------------------------------------------------//
    D3D11_BUFFER_DESC ib_desc = {};
    ib_desc.ByteWidth = sizeof(index_data);
    ib_desc.Usage = D3D11_USAGE_IMMUTABLE;
    ib_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    D3D11_SUBRESOURCE_DATA sub_index_data = { index_data };
    hr = render_->device_->CreateBuffer(&ib_desc, &sub_index_data, &index_buffer_);
    if (FAILED(hr)) { return hr; }
    //--------------------------------------------------//
    return hr;
}

