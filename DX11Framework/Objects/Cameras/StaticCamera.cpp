#include "StaticCamera.h"

XMFLOAT4X4 StaticCamera::SetView()
{
    XMStoreFloat4x4(&view_, XMMatrixLookAtLH(XMLoadFloat3(&data_->eye), XMLoadFloat3(&data_->at), XMLoadFloat3(&data_->up)));
    return view_;
}

void StaticCamera::Update(const float& delta_time)
{
    view_ = SetView();
    // --- 64 bits - 16 bytes --- //
    cb_data_.SetView(XMMatrixTranspose(XMLoadFloat4x4(&view_)));
}
