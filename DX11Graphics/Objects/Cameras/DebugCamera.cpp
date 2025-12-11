#include "DebugCamera.h"

#include <random>

// TODO (Debug camera) : Continue the process of implementing a functional camera.

DebugCamera::DebugCamera()
{
    // -- Converting XMFLOAT3 into XMVECTOR for calculations -- //
    XMVECTOR eye = XMLoadFloat3(&data_->eye);
    XMVECTOR at = XMLoadFloat3(&data_->at);
    XMVECTOR up = XMLoadFloat3(&data_->up);
    
    motion_ = new MotionData();
    pos_ = at;
    look_ = XMVector3Normalize(XMVectorSubtract(at, eye));
    right_ = XMVector3Normalize(XMVector3Cross(up, look_));
    up_ = XMVector3Cross(look_, right_);
}

DebugCamera::~DebugCamera()
{
    motion_ = nullptr;
}

void DebugCamera::Walk(const float& velocity)
{
    pos_ += velocity * look_;
    XMVECTOR v = XMVectorReplicate(velocity);
    XMStoreFloat3(&motion_->position, XMVectorMultiplyAdd(pos_, look_, v));
}

void DebugCamera::Strafe(const float& velocity)
{
    pos_ += velocity * right_;
    XMVECTOR v = XMVectorReplicate(velocity);
    XMStoreFloat3(&motion_->position, XMVectorMultiplyAdd(pos_, right_, v));
}

void DebugCamera::Elevation(const float& velocity)
{
    pos_ += velocity * up_;
    XMVECTOR v = XMVectorReplicate(velocity);
    XMStoreFloat3(&motion_->position, XMVectorMultiplyAdd(pos_, up_, v));
}


void DebugCamera::Yaw(const float& rotation)
{
    XMVECTOR q_rotation = XMQuaternionRotationAxis(up_, rotation); 
    XMMATRIX rotate = XMMatrixRotationQuaternion(q_rotation); 
    up_ = XMVector3TransformNormal(up_, rotate);
    right_ = XMVector3TransformNormal(right_, rotate);
    look_ = XMVector3TransformNormal(look_, rotate);
}

void DebugCamera::Pitch(const float& rotation)
{
    XMVECTOR q_rotation = XMQuaternionRotationAxis(right_, rotation);
    XMMATRIX rotate = XMMatrixRotationQuaternion(q_rotation);
    right_ = XMVector3TransformNormal(right_, rotate);
    look_ = XMVector3TransformNormal(look_, rotate);
}

XMFLOAT4X4 DebugCamera::SetView()
{
    XMVECTOR look = XMVector3Normalize(look_);
    XMVECTOR up = XMVector3Normalize(XMVector3Cross(look_, right_));
    XMVECTOR right = XMVector3Cross(look_, up_);
    
    XMStoreFloat3(&motion_->position, pos_);
    XMStoreFloat3(&motion_->look, look);
    XMStoreFloat3(&motion_->right, right);
    XMStoreFloat3(&motion_->up, up);

    XMStoreFloat3(&data_->at, XMLoadFloat3(&motion_->position));
    XMStoreFloat4x4(&view_, XMMatrixLookToLH(XMLoadFloat3(&motion_->position), XMLoadFloat3(&motion_->look), XMLoadFloat3(&motion_->up)));
    return view_;
}

void DebugCamera::Update(const float& delta_time)
{
    #pragma region User Input Handling 
    if (GetAsyncKeyState('W') & 0x8000)
    {
        Walk(speed_);
    }
    if (GetAsyncKeyState('S') & 0x8000)
    {
        Walk(-speed_);
    }
    if (GetAsyncKeyState('A') & 0x8000)
    {
        Strafe(-speed_);
    }
    if (GetAsyncKeyState('D') & 0x8000)
    {
        Strafe(speed_);
    }
    if (GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        Elevation(speed_);
    }
    if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
    {
        Elevation(-speed_);
    }
    if (GetAsyncKeyState('H') & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        Yaw(rotation_);
    }
    if (GetAsyncKeyState('F') & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8000)
    {
        Yaw(-rotation_);
    }
    if (GetAsyncKeyState('G') & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000)
    {
        Pitch(rotation_);
    }
    if (GetAsyncKeyState('T') & 0x8000 || GetAsyncKeyState(VK_UP) & 0x8000)
    {
        Pitch(-rotation_);
    }
    #pragma endregion
    
    view_ = SetView();
    // --- 16 | 12 - 4 | 3 --- //
    cb_data_.SetView(XMMatrixTranspose(XMLoadFloat4x4(&view_)));
    cb_data_.SetCameraPosition(data_->at);
}
