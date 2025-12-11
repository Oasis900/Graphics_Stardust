#pragma once
#ifndef POSITION_DATA_H
#define POSITION_DATA_H

#include <DirectXMath.h>
#include <iostream>
#include <random>

#pragma region DirectX Managed
using DirectX::XMFLOAT4X4;
using DirectX::XMFLOAT4;
using DirectX::XMMatrixScaling;
using DirectX::XMMatrixRotationX;
using DirectX::XMMatrixRotationY;
using DirectX::XMMatrixScaling;
using DirectX::XMMatrixRotationRollPitchYaw;
using DirectX::XMConvertToRadians;
using DirectX::XMMatrixTranslation;
#pragma endregion

enum BodyType
{
    SUN,
    PLANET,
    MOON,
    STATION,
    BACKDROP
};

struct PositionData
{
protected:
    XMFLOAT4X4 pos_;
    float delta_time_;

    // TODO (Object Position Update) : Manipulate rotation data via JSON
    
public:
    PositionData(const float& delta_time, const XMFLOAT4X4& in_pos)
    {
        pos_ = in_pos;
        delta_time_ = delta_time;
    }
    virtual XMFLOAT4X4 GetPos() {return pos_;}
};

struct Sun : PositionData
{
    Sun(const float& delta_time, const XMFLOAT4X4& in_pos) : PositionData(delta_time, in_pos)
    {
        XMStoreFloat4x4(&pos_, XMMatrixScaling(3,3,3)
                    * XMMatrixRotationY(delta_time_/6)
                    * XMMatrixRotationRollPitchYaw(XMConvertToRadians(-25), 0, XMConvertToRadians(25)));
    }
};

struct Planet : PositionData
{
    Planet(const float& delta_time, const XMFLOAT4X4& in_pos, const XMFLOAT3& offset) : PositionData(delta_time, in_pos)
    {
        XMStoreFloat4x4(&pos_, XMMatrixScaling(0.6f,0.6f,0.6f)
                    * XMMatrixRotationRollPitchYaw(XMConvertToRadians(-25), 0, XMConvertToRadians(25))
                    * XMMatrixRotationY(-delta_time_)
                    * XMMatrixTranslation(offset.x, offset.y, offset.z)
                    * XMMatrixRotationY(delta_time_ * offset.y/4));
    }
};

struct Moon : PositionData
{
private:
    XMFLOAT4X4 moon_;
    
public:
    Moon(const float& delta_time, const XMFLOAT4X4& in_pos, const XMFLOAT3& offset) : PositionData(delta_time, in_pos)
    {
        XMStoreFloat4x4(&moon_, XMMatrixScaling(0.3f,0.3f,0.3f)
                     * XMMatrixTranslation(offset.x, offset.y, offset.z)
                     * XMMatrixRotationY(delta_time_*1.5f)
                     * XMLoadFloat4x4(&pos_));
    }
    XMFLOAT4X4 GetPos() final {return moon_;}
};

struct Station : PositionData
{
    Station(const float& delta_time, const XMFLOAT4X4& in_pos) : PositionData(delta_time, in_pos)
    {
        XMStoreFloat4x4(&pos_, XMMatrixScaling(1,1,1)
            * XMMatrixRotationY(sin(delta_time_*2) + cos(delta_time_))
            * XMMatrixTranslation(0, -10, 0)
            * XMMatrixRotationX(XMConvertToRadians(180)));
    }
};

struct Backdrop : PositionData
{
    Backdrop(const float& delta_time, const XMFLOAT4X4& in_pos, const XMFLOAT3& offset) : PositionData(delta_time, in_pos)
    {
        DirectX::XMStoreFloat4x4(&pos_, XMMatrixScaling(100.0f,100.0f,100.0f)
                * XMMatrixTranslation(offset.x, offset.y, offset.z));
    }
};


#endif
