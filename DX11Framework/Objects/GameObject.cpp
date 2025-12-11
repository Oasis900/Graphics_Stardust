#include "GameObject.h"

#include <iostream>

#include "../Structures/Components.h"

void GameObject::Draw_()
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
    render_->device_context_->IASetVertexBuffers(0, 1, &mesh_->VertexBuffer, &render_->stride_, &render_->offset_);
    render_->device_context_->IASetIndexBuffer(mesh_->IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    render_->device_context_->DrawIndexed(mesh_->IndexCount, 0, 0);
}

void GameObject::Texture_()
{
    // -- ! Non-functional ! -- //
    render_->device_context_->PSSetShaderResources(1 ,1, &texture_);
    // --- 16 - 4 --- //
    cb_data_.SetHasTexture(true);
}

GameObject::~GameObject()
{
    if (texture_) texture_->Release();
    if (mesh_) mesh_ = nullptr;
    if (loading_) loading_ = nullptr;
    if (pos_) pos_ = nullptr;
}

GameObject::GameObject(RenderData* render_in, const std::string& mesh_path, const std::string& texture_path, const BodyType& type)
{
    loading_ = new Loading();
    render_ = render_in;
    type_ = type;
    render_->topology_flag_ = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    mesh_ = loading_->LoadMesh(render_in->device_, mesh_path);
    texture_ = loading_->LoadTexture(render_in->device_, texture_path);
    Texture_();
}

void GameObject::Render(){Draw_();}

void GameObject::Update(const float& delta_time)
{
    #pragma region Object World Position Update
    switch (type_)
    {
        case SUN:
            pos_ = new Sun(delta_time, world_pos_);
            break;

        case PLANET:
            pos_ = new Planet(delta_time, world_pos_, {-10, 2, 0});
            break;

        case MOON:
            pos_ = new Planet(delta_time, world_pos_, {-10, 2, 0});
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
