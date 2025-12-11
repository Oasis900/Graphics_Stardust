#pragma once
#ifndef RENDERDATA_H
#define RENDERDATA_H
#include <d3d11.h>
#include "SimpleVertex.h"

#pragma region Type definitions
typedef ID3D11DeviceContext device_context;
typedef ID3D11Device device;
typedef ID3D11Buffer constant_buffer;
typedef D3D11_PRIMITIVE_TOPOLOGY topology_;
#pragma endregion

struct RenderData
{
    D3D11_MAPPED_SUBRESOURCE mappedSubresource_;
    UINT stride_ = {sizeof(SimpleVertex)};
    UINT offset_ =  0;
    //--------------------------------------------------//
    topology_ topology_flag_ = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
    //--------------------------------------------------//
    device_context* device_context_ = nullptr;
    device* device_ = nullptr;
    constant_buffer* constant_buffer_ = nullptr;

    ~RenderData()
    {
        if(device_context_) device_context_->Release();
        if(device_) device_->Release();
        if(constant_buffer_) constant_buffer_->Release();
    }
};

#endif
