#pragma once
#ifndef OBJ_GAMEOBJECT_H
#define OBJ_GAMEOBJECT_H

#include "../Loading.h"
#include "../Structures/Components.h"
#include "../Structures/ConstantBufferData.h"
#include "../Structures/Interfaces.h"
#include "../Structures/PositionData.h"
#include "../Structures/RenderData.h"

class GameObject : IRenderable, IUpdatable
{
    ConstantBufferData& cb_data_ = ConstantBufferData::GetInstance();
    //--------------------------------------------------//
    ID3D11ShaderResourceView* texture_ = nullptr;
    Loading* loading_ = nullptr;
    RenderData* render_ = nullptr;
    MeshData* mesh_ = nullptr;
    PositionData* pos_ = nullptr;
    //--------------------------------------------------//
    BodyType type_;
    XMFLOAT4X4 world_pos_;
    //--------------------------------------------------//
    void Draw_();
    void Texture_();
    //--------------------------------------------------//
    ~GameObject();
    
public:
    GameObject(RenderData* render_in, const std::string& mesh_path, const std::string& texture_path, const BodyType& type);
    //--------------------------------------------------//
    /*RenderData* GetRenderData() const {return render_;}
    ID3D11ShaderResourceView* GetShaderResource() const {return texture_;}
    MeshData* GetMeshData() const {return mesh_;}
    XMFLOAT4X4 GetWorldMatrix() const {return world_pos_;}*/
    //--------------------------------------------------//
    void Render() final;
    void Update(const float& delta_time) final;

};
#endif