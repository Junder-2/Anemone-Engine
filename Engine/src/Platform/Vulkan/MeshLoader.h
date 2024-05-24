#pragma once

#include <assimp/Importer.hpp>
#include <assimp/mesh.h>

#include "ANE/Renderer/Mesh.h"

namespace Vulkan
{
    using namespace Engine;

    struct BoundingBox
    {
        Vector3 Min = Vector3(FLT_MAX);
        Vector3 Max = Vector3(-FLT_MAX);
        Vector3 Size;
    };

    class MeshLoader
    {
    public:
        static MeshAsset* LoadMesh(const char* path);

        static MeshAsset* LoadMesh(const char* path, int flags);

    public:
        static Assimp::Importer Importer;

    private:
        static MeshAsset* InitScene(const aiScene* scene);

        static Mesh InitMesh(const aiMesh* mesh, const aiScene* scene);
    };
}
