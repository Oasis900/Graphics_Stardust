#pragma once
#ifndef DX11_LIGHTINGDATA_H
#define DX11_LIGHTINGDATA_H
#include <DirectXMath.h>
#include "ConstantBufferData.h"
using DirectX::XMFLOAT4;
using DirectX::XMFLOAT3;
// TODO (Light Data) : Expand upon data assignment, implement values assignment from JSON file

struct Diffuse
{
private:
    XMFLOAT4 light_ = XMFLOAT4(0.949f, 0.776f, 0.345f, 1.0f);
    XMFLOAT4 material_ = XMFLOAT4(0.8f, 0.8f, 0.8f, 0.8f);

public:
    Diffuse(ConstantBufferData& cb_data)
    {
        cb_data.SetDiffuseLight(light_);
        cb_data.SetDiffuseMaterial(material_);
    }
};

struct Ambient
{
private:
    XMFLOAT4 light_ = XMFLOAT4(0.02f, 0.02f, 0.02f, 0.02f);
    XMFLOAT4 material_ = XMFLOAT4(0.4f, 0.3f, 0.045f, 0.5f);

public:
    Ambient(ConstantBufferData& cb_data)
    {
        // --- 16 | 16 - 4 | 4 --- //
        cb_data.SetAmbientLight(light_);
        cb_data.SetAmbientMaterial(material_);
    }
};

struct Specular
{
private:
    XMFLOAT4 light_ = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    XMFLOAT4 material_ = XMFLOAT4(1.0f,1.0f,1.0f, 1.0f);
    float power_ = 10.0f;

public:
    Specular(ConstantBufferData& cb_data)
    {
        // --- 16 | 16 | 4 - 4 | 4 | 1 --- //
        cb_data.SetSpecularLight(light_);
        cb_data.SetSpecularMaterial(material_);
        cb_data.SetSpecularPower(power_);
    }
};

struct PointLight
{
private:
    XMFLOAT4 color_ = XMFLOAT4(0.902f, 0.718f, 0.41f, 1.08f);
    XMFLOAT3 pos_ = XMFLOAT3(0.0f, 0.0f, 0.0f);
    float range = 2.0f;

public:
    PointLight(ConstantBufferData& cb_data)
    {
        // --- 12 | 4 | 16 - 3 | 1 | 4 --- //
        cb_data.SetPointColor(color_);
        cb_data.SetPointPosition(pos_);
        cb_data.SetPointRange(range);
    }
};

struct DirectionalLight
{
private:
    XMFLOAT3 direction_ = XMFLOAT3(0, 0, 10.0);
    
public:
    DirectionalLight(ConstantBufferData& cb_data)
    {
        // --- 12 | 4 - 3 | 1 --- //
        cb_data.SetLightDirection(direction_);
    }
};

struct LightData
{
private:
    ConstantBufferData& cb_data_ = ConstantBufferData::GetInstance();
    //--------------------------------------------------//
    Diffuse* diffuse_ = nullptr;
    Ambient* ambient_ = nullptr;
    Specular* specular_ = nullptr;
    PointLight* point_ = nullptr;
    DirectionalLight* directional_ = nullptr;

public:
    LightData()
    {
        diffuse_ = new Diffuse(cb_data_);
        ambient_ = new Ambient(cb_data_);
        specular_ = new Specular(cb_data_);
        point_ = new PointLight(cb_data_);
        directional_ = new DirectionalLight(cb_data_);
    }
    ~LightData()
    {
        if (diffuse_) diffuse_ = nullptr;
        if (ambient_) ambient_ = nullptr;
        if (specular_) specular_ = nullptr;
        if (point_) point_ = nullptr;
        if (directional_) directional_ = nullptr;
    }
};

#endif

