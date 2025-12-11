#pragma once
#ifndef HARDOBJ_PYRAMID_H
#define HARDOBJ_PYRAMID_H
#include "BaseHardObject.h"


class Pyramid : public BaseHardObject
{
    HRESULT InitialiseBuffers() final;
    HRESULT CreateVertexBuffer(HRESULT& hr) final;
    HRESULT CreateIndexBuffer(HRESULT& hr) final;

public:
    Pyramid(RenderData* render_in, const BodyType& type);
};
#endif

