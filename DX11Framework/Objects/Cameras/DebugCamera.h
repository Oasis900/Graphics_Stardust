#pragma once
#ifndef OBJ_CAM_DEBUGCAMERA_H
#define OBJ_CAM_DEBUGCAMERA_H
#include "BaseCamera.h"
#include <windows.h>

struct MotionData
{
    XMFLOAT3 position;
    XMFLOAT3 right;
    XMFLOAT3 up;
    XMFLOAT3 look;
};

class DebugCamera : public BaseCamera
{
    MotionData* motion_ = nullptr;
    //--------------------------------------------------//
    float speed_ = 0.005f;
    float rotation_ = speed_/8.0f;
    //--------------------------------------------------//
    XMVECTOR pos_;
    XMVECTOR look_;
    XMVECTOR right_;
    XMVECTOR up_;
    //--------------------------------------------------//
    void Walk(const float& velocity);
    void Strafe(const float& velocity);
    void Elevation(const float& velocity);
    //--------------------------------------------------//
    void Yaw(const float& rotation);
    void Pitch(const float& rotation);
    //--------------------------------------------------//
    ~DebugCamera();
public:
    DebugCamera();
    //--------------------------------------------------//
    XMFLOAT4X4 SetView() final;
    void Update(const float& delta_time) final;
};
#endif