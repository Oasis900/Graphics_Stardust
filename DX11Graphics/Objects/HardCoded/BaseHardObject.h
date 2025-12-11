#pragma once
#ifndef HARDOBJ_BASE_H
#define HARDOBJ_BASE_H

#include <d3d11.h>
#include <windows.h>
#include <DirectXMath.h>

#pragma region Structure Includes
#include "../../Structures/Interfaces.h"
#include "../../Structures/ConstantBufferData.h"
#include "../../Structures/PositionData.h"
#include "../../Structures/RenderData.h"
#include "../../Loading.h"
#include "../../Structures/Components.h"
#pragma endregion
using DirectX::XMFLOAT2;

class BaseHardObject : IRenderable, IUpdatable
{
    Loading* loading_ = nullptr;
    ID3D11ShaderResourceView* texture_ = nullptr;
    ID3D11Texture2D* skybox_ = nullptr;
    ConstantBufferData& cb_data_ = ConstantBufferData::GetInstance();
    PositionData* pos_ = nullptr;
    //--------------------------------------------------//
    XMFLOAT4X4 world_pos_;
    //--------------------------------------------------//
    void Draw_() const;
    void Texture_();

protected:
    RenderData* render_ = nullptr;
    ID3D11Buffer* vertex_buffer_ = nullptr;
    ID3D11Buffer* index_buffer_ = nullptr;
    //--------------------------------------------------//
    BodyType type_;
    unsigned int index_count_;
    //--------------------------------------------------//
    // --- Abstract Functions for Children --- //
    virtual HRESULT InitialiseBuffers() = 0;
    virtual HRESULT CreateVertexBuffer(HRESULT& hr) = 0;
    virtual HRESULT CreateIndexBuffer(HRESULT& hr) = 0;
    //--------------------------------------------------//
    ~BaseHardObject();
    
public:
    BaseHardObject(RenderData* render_in, const BodyType& type);
    //--------------------------------------------------//
    RenderData* GetRenderData() const {return render_;}
    ID3D11Buffer* const* GetVertexBuffer() const {return &vertex_buffer_;}
    ID3D11Buffer* GetIndexBuffer() const {return index_buffer_;}
    //--------------------------------------------------//
    unsigned int GetIndexCount() const {return index_count_;}
    XMFLOAT4X4 GetWorldPos() const {return world_pos_;}
    //--------------------------------------------------//
    ID3D11ShaderResourceView** GetTexture() {return &texture_;}
    void SetTexture(ID3D11ShaderResourceView* texture) {texture_ = texture;}
    //--------------------------------------------------//
    void Render() final;
    void Update(const float& delta_time) final;

};
#endif