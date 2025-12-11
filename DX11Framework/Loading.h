#pragma once
#ifndef DX11_LOADING_H
#define DX11_LOADING_H
#include <DirectXMath.h>
#include <d3d11_1.h>

#include "Objects/OBJLoader.h"
#include "Textures/DDSTextureLoader.h"
//#include <nlohmann/json.hpp>
#include <vector>

//using json = nlohmann::json; - nlohmann library not found

typedef ID3D11ShaderResourceView Texture;

class Loading
{
    MeshData* mesh_ = nullptr;
    Texture* texture_ = nullptr;
    
    public:
    MeshData* LoadMesh(ID3D11Device* device, const std::string& path);
    Texture* LoadTexture(ID3D11Device* device, const std::string& path);
    //json* LoadJSON(const std::string& path);
    ~Loading();
};
#endif