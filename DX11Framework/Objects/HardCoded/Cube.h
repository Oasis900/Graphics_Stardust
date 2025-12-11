#pragma once
#ifndef HARDOBJ_CUBE_H
#define HARDOBJ_CUBE_H
#include "BaseHardObject.h"

class Cube : public BaseHardObject
{
    HRESULT InitialiseBuffers() final;
    HRESULT CreateVertexBuffer(HRESULT& hr) final;
    HRESULT CreateIndexBuffer(HRESULT& hr) final;

public:
    Cube(RenderData* render_in, const BodyType& type);
};
#endif

