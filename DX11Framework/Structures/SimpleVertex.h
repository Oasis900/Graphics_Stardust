#pragma once
#ifndef STRUCT_SIMPLEVERTEX_H
#define STRUCT_SIMPLEVERTEX_H
#include <cstring>
#include <DirectXMath.h>
using DirectX::XMFLOAT3;
using DirectX::XMFLOAT2;

struct SimpleVertex
{
    XMFLOAT3 Pos;
    XMFLOAT3 Normal;
    XMFLOAT2 TexC;
    //--------------------------------------------------//
    bool operator<(const SimpleVertex other) const
    {
        return memcmp((void*)this, (void*)&other, sizeof(SimpleVertex)) > 0;
    };
};
#endif