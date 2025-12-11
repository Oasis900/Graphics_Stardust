#pragma once
#ifndef COMPONENTS_H
#define COMPONENTS_H
#include "../Objects/GameObject.h"
#include "../Objects/HardCoded/BaseHardObject.h"

// -- ! Not in Use ! -- //
class Component
{
public:
    virtual ~Component() = default;
};

class RenderComponent
{
private:
    ConstantBufferData& cb_data_ = ConstantBufferData::GetInstance();
    RenderData* render_;
public:
    /*void Draw(const GameObject* obj);
    void Draw(const BaseHardObject* obj);*/
};
// -- ! Not in Use ! -- //
#endif
