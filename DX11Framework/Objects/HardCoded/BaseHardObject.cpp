#include "BaseHardObject.h"

#include <iostream>

BaseHardObject::~BaseHardObject()
{
    if (render_) render_ = nullptr;
    if (pos_) pos_ = nullptr;
    if (vertex_buffer_) vertex_buffer_ = nullptr;
    if (index_buffer_) index_buffer_ = nullptr;
    if (loading_) loading_ = nullptr;
    if (texture_) texture_->Release();
}

BaseHardObject::BaseHardObject(RenderData* render_in, const BodyType& type)
{
    type_ = type;
    render_ = render_in;
    loading_ = new Loading();
    render_->topology_flag_ = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    Texture_();
}

void BaseHardObject::Update(const float& delta_time)
{
    #pragma region Object World Position Update
    switch (type_)
    {
        case SUN:
            pos_ = new Sun(delta_time, world_pos_);
            break;

        case PLANET:
            pos_ = new Planet(delta_time, world_pos_, {-20, 0, 0});
            break;

        case MOON:
            pos_ = new Planet(delta_time, world_pos_, {-20, 0, 0});
            world_pos_ = pos_->GetPos();
            pos_ = new Moon(delta_time, world_pos_, {-3, 0, 0});
            break;

        case STATION:
            pos_ = new Station(delta_time, world_pos_);
            break;

        case BACKDROP:
            pos_ = new Backdrop(delta_time, world_pos_, cb_data_.GetCameraPosition());
            break;

        default:
            std::cerr<<"!! Error : Unknown Cube Object type\n";
            break;
    }
    #pragma endregion

    if (pos_) world_pos_ = pos_->GetPos();
    // --- 64 bits - 16 bytes --- Sets World Matrix according to object World Matrix --- // 
    cb_data_.SetWorld(XMMatrixTranspose(XMLoadFloat4x4(&world_pos_)));
}

void BaseHardObject::Draw_() const
{
    if (render_->topology_flag_ != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
    {
        render_->device_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }
    //--------------------------------------------------//
    render_->device_context_->Map(render_->constant_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &render_->mappedSubresource_);
    cb_data_.SetWorld(XMMatrixTranspose(XMLoadFloat4x4(&world_pos_)));
    memcpy(render_->mappedSubresource_.pData, &cb_data_, sizeof(cb_data_));
    render_->device_context_->Unmap(render_->constant_buffer_, 0);
    //--------------------------------------------------//
    render_->device_context_->IASetVertexBuffers(0, 1, &vertex_buffer_, &render_->stride_, &render_->offset_);
    render_->device_context_->IASetIndexBuffer(index_buffer_, DXGI_FORMAT_R16_UINT, 0);
    render_->device_context_->DrawIndexed(index_count_, 0, 0);
}

void BaseHardObject::Texture_()
{
    switch (type_)
    {
    case BACKDROP:
        texture_ = loading_->LoadTexture(render_->device_, "Textures\\Skybox_COLOR.dds");
        render_->device_context_->PSSetShaderResources(2, 1, &texture_);
        // --- 16 - 4 --- //
        cb_data_.SetHasTexture(true);
        break;

    case PLANET:
        texture_ = loading_->LoadTexture(render_->device_, "Textures\\asphalt_COLOR.dds");
        render_->device_context_->PSSetShaderResources(1, 1, &texture_);
        // --- 16 - 4 --- //
        cb_data_.SetHasTexture(true);
        break;
        
    default:
        texture_ = loading_->LoadTexture(render_->device_, "Textures\\Crate_COLOR.dds");
        render_->device_context_->PSSetShaderResources(0, 1, &texture_);
        // --- 16 - 4 --- //
        cb_data_.SetHasTexture(true);
        break;
    }
}

void BaseHardObject::Render(){Draw_();}
