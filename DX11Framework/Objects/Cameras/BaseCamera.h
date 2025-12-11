#pragma once
#ifndef BASE_CAMERA_H
#define BASE_CAMERA_H
#include "../../Structures/ConstantBufferData.h"
#include "../../Structures/Interfaces.h"
#include <DirectXMath.h>
#include <algorithm>
using namespace DirectX;

struct CameraData
{
    XMFLOAT3 eye;
    XMFLOAT3 at;
    XMFLOAT3 up;
};

class BaseCamera : IUpdatable
{
    // -- Window related variables to calculate project matrix -- //
    float window_width_ = 1280.0f;
    float window_height_ = 768.0f;
    float near_depth_ = 0.01f;
    float far_depth_ = 10000.0f;
    float fov_ = 90.0f;
    float aspect_ = window_width_/window_height_;

protected:
    // -- Member variables for inheritance -- //
    ConstantBufferData& cb_data_ = ConstantBufferData::GetInstance();
    CameraData* data_ = nullptr;
    XMFLOAT4X4 projection_;
    XMFLOAT4X4 view_;
    //--------------------------------------------------//
    float GetFOV() const {return fov_;}
    void SetFOV(const float& fov){fov_ = fov;}
    //--------------------------------------------------//
    // -- Calculates projection and stores to Constant Buffer -- //
    void SetLens();

public:
    BaseCamera();
    //--------------------------------------------------//
    XMVECTOR GetEyeVec() const {return XMLoadFloat3(&data_->eye);}
    XMFLOAT3 GetEye() const {return data_->eye;}
    XMVECTOR GetAtVec() const {return XMLoadFloat3(&data_->at);}
    XMFLOAT3 GetAt() const {return data_->at;}
    XMVECTOR GetUpVec() const {return XMLoadFloat3(&data_->up);}
    XMFLOAT3 GetUp() const {return data_->up;}
    //--------------------------------------------------//
    float GetNearDepth() const {return near_depth_;}
    float GetFarDepth() const {return far_depth_;}
    float GetFov() const {return fov_;}
    float GetAspect() const {return aspect_;}
    float GetWindowWidth() const {return window_width_;}
    float GetWindowHeight() const {return window_height_;}
    //--------------------------------------------------//
    XMFLOAT4X4* GetView() {return &view_;}
    XMFLOAT4X4* GetProjection() {return &projection_;}
    //--------------------------------------------------//
    virtual XMFLOAT4X4 SetView() = 0;
    void Update(const float& delta_time) override = 0;
    
    ~BaseCamera();
};
#endif
