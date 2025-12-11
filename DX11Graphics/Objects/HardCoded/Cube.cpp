#include "Cube.h"
#include <iostream>
#include <ostream>

Cube::Cube(RenderData* render_in, const BodyType& type) : BaseHardObject(render_in, type)
{
    index_count_ = 36;
    InitialiseBuffers();
}

HRESULT Cube::InitialiseBuffers()
{
    HRESULT hr;
    
    CreateVertexBuffer(hr);
    if (FAILED(hr)) return hr;
    //--------------------------------------------------//
    CreateIndexBuffer(hr);
    if (FAILED(hr)) return hr;
    
    return S_OK;
}

HRESULT Cube::CreateVertexBuffer(HRESULT& hr)
{
    SimpleVertex vertex_data[] = 
    {
        // Positions                                     // Tex Coords                          // Normals             
        { XMFLOAT3(-1.00f,  1.00f, -1),XMFLOAT3(-1.00f,  1.00f, -1), XMFLOAT2(0.0f, 0.0f)},   // Front-Top-Left
        
        { XMFLOAT3(1.00f,  1.00f, -1),XMFLOAT3(1.00f,  1.00f, -1), XMFLOAT2(1.0f, 0.0f)},     // Front-Top-Right
        
        { XMFLOAT3(-1.00f, -1.00f, -1),XMFLOAT3(-1.00f, -1.00f, -1), XMFLOAT2(0.0f, 1.0f)},  // Front-Bottom-Left
        
        { XMFLOAT3(1.00f, -1.00f, -1),XMFLOAT3(1.00f, -1.00f, -1), XMFLOAT2(1.0f, 1.0f)},    // Front-Bottom-Right
        
        { XMFLOAT3(-1.00f,  1.00f, 1),XMFLOAT3(-1.00f,  1.00f, 1), XMFLOAT2(1.0f, 0.0f)},     // Back-Top-Left
            
        { XMFLOAT3(1.00f,  1.00f, 1),XMFLOAT3(1.00f,  1.00f, 1), XMFLOAT2(0.0f, 0.0f)},       // Back-Top-Right
        
        { XMFLOAT3(-1.00f, -1.00f, 1),XMFLOAT3(-1.00f, -1.00f, 1), XMFLOAT2(1.0f, 1.0f)},    // Back-Bottom-Left
        
        { XMFLOAT3(1.00f, -1.00f, 1),XMFLOAT3(1.00f, -1.00f, 1), XMFLOAT2(0.0f, 1.0f)},      // Back-Bottom-Right
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

HRESULT Cube::CreateIndexBuffer(HRESULT& hr)
{
    if (type_ != BACKDROP)
    {
        WORD index_data[] =
        {
            //Front
            2, 0, 1, 1, 3, 2,
            //Back
            6, 7, 5, 5, 4, 6,
            //Right
            5, 7, 3, 3, 1, 5,
            //Left
            6, 4, 0, 0, 2, 6,
            //Top
            2, 3, 7, 7, 6, 2,
            //Bottom
            0, 4, 1, 1, 4, 5
        };
        //--------------------------------------------------//
        D3D11_BUFFER_DESC ib_desc = {};
        ib_desc.ByteWidth = sizeof(index_data);
        ib_desc.Usage = D3D11_USAGE_IMMUTABLE;
        ib_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        D3D11_SUBRESOURCE_DATA sub_index_data = { index_data };
        //--------------------------------------------------//
        hr = render_->device_->CreateBuffer(&ib_desc, &sub_index_data, &index_buffer_);
    }
    // -- For skybox rendering, cube is inverted -- //
    else if (type_ == BACKDROP)
    {
        WORD index_data[] =
       {
            //Front
            1, 0, 2, 2, 3, 1,
            //Back
            5, 7, 6, 6, 4, 5,
            //Right
            3, 7, 5, 5, 1, 3,
            //Left
            0, 4, 6, 6, 2, 0,
            //Top
            7, 3, 2, 2, 6, 7,
            //Bottom
            1, 4, 0, 1, 5, 4
        };
        //--------------------------------------------------//
        D3D11_BUFFER_DESC ib_desc = {};
        ib_desc.ByteWidth = sizeof(index_data);
        ib_desc.Usage = D3D11_USAGE_IMMUTABLE;
        ib_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        D3D11_SUBRESOURCE_DATA sub_index_data = { index_data };
        //--------------------------------------------------//
        hr = render_->device_->CreateBuffer(&ib_desc, &sub_index_data, &index_buffer_);
    }
    

    if (FAILED(hr)) { return hr; }
    return hr;
}

