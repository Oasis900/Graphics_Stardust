#pragma once
#include "BaseCamera.h"
using DirectX::XMMatrixLookAtLH;

class StaticCamera : public BaseCamera
{
public:
    XMFLOAT4X4 SetView() final;
    void Update(const float& delta_time) final;
};
