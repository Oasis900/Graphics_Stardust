#include "Components.h"

/*
void RenderComponent::Draw(const GameObject* obj)
{
    render_ = obj->GetRenderData();
    
    if (render_->topology_flag_ != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
    {
        render_->device_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }
    
    render_->device_context_->Map(render_->constant_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &render_->mappedSubresource_);
    cb_data_.SetWorld(XMMatrixTranspose(XMLoadFloat4x4(obj->GetWorldMatrix())));
    memcpy(render_->mappedSubresource_.pData, &cb_data_, sizeof(cb_data_));
    render_->device_context_->Unmap(render_->constant_buffer_, 0);
    
    render_->device_context_->IASetVertexBuffers(0, 1, &obj->GetMeshData()->VertexBuffer,
        &obj->GetMeshData()->VBStride, &obj->GetMeshData()->VBOffset);
    render_->device_context_->IASetIndexBuffer(obj->GetMeshData()->IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
    render_->device_context_->DrawIndexed(obj->GetMeshData()->IndexCount, 0, 0);
}

void RenderComponent::Draw(const BaseHardObject* obj)
{
    render_ = obj->GetRenderData();
    
    if (render_->topology_flag_ != D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
    {
        render_->device_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }
    
    render_->device_context_->Map(render_->constant_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0,
        &render_->mappedSubresource_);
    cb_data_.SetWorld(XMMatrixTranspose(XMLoadFloat4x4(obj->GetWorldPos())));
    memcpy(render_->mappedSubresource_.pData, &cb_data_, sizeof(cb_data_));
    render_->device_context_->Unmap(render_->constant_buffer_, 0);
    
    render_->device_context_->IASetVertexBuffers(0, 1, obj->GetVertexBuffer(), &render_->stride_, &render_->offset_);
    render_->device_context_->IASetIndexBuffer(obj->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);
    render_->device_context_->DrawIndexed(obj->GetIndexCount(), 0, 0);
}*/
