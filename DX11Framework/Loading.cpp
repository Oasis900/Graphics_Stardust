#include "Loading.h"

MeshData* Loading::LoadMesh(ID3D11Device* device, const std::string& path)
{
    mesh_ = new MeshData();

    // TODO (LoadMesh) : May need to set invert Tex Coords to true or leave it empty.
    *mesh_ = OBJLoader::Load(path, device, true);

    if (mesh_->IndexCount == 0)
    {
        throw std::runtime_error("File path for mesh potentially incorrect\n");
    }

    return mesh_;
}

Texture* Loading::LoadTexture(ID3D11Device* device, const std::string& path)
{
    // Converts incoming path string to a wide string
    std::wstring ws_path = std::wstring(path.begin(), path.end());

    // Converts the characters within the wide string into wide characters
    const wchar_t* wc_path = ws_path.c_str(); 

    // Passes through the path reference as wide characters
    CreateDDSTextureFromFile(device, wc_path, nullptr, &texture_);

    if (texture_ == nullptr)
    {
        // Likely to be thrown from Human Error of the path 
        throw std::runtime_error("Loading texture is not initialized\n");
    }

    return texture_;
}

/*json* Loading::LoadJSON(const std::string& path)
{
    return nullptr;
}*/


Loading::~Loading()
{
    if (mesh_) mesh_ = nullptr;
    if (texture_) texture_ = nullptr;
}
