#include "BaseCamera.h"

void BaseCamera::SetLens()
{
    // -- Matrix used for arithmetic calculations -- //
    XMMATRIX perspective = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov_), aspect_, near_depth_, far_depth_);
    // -- Place matrix calculation data into suitable long term data format -- //
    XMStoreFloat4x4(&projection_, perspective);
    // --- 64 bits - 16 bytes --- //
    cb_data_.SetProjection(XMMatrixTranspose(XMLoadFloat4x4(&projection_)));
}

BaseCamera::BaseCamera()
{
    SetLens();
    //--------------------------------------------------//
    data_ = new CameraData();
    data_->eye = XMFLOAT3(0,0,-20);
    data_->at = XMFLOAT3(0.0f, 0.0f, 0.0f);
    data_->up = XMFLOAT3(0.0f, 1.0f, 0.0f);
}

BaseCamera::~BaseCamera()
{
    if (data_) data_ = nullptr;
}

