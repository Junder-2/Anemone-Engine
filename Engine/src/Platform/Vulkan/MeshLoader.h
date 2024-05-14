#pragma once

#include <assimp/Importer.hpp>
#include <assimp/mesh.h>

#include "ANE/Renderer/Mesh.h"

namespace Vulkan
{
    struct BoundingBox
    {
        glm::vec3 Min = glm::vec3(FLT_MAX);
        glm::vec3 Max = glm::vec3(-FLT_MAX);
        glm::vec3 Size;
    };

    class MeshLoader
    {
    public:
        static MeshAsset LoadMesh(const char* path);

        static MeshAsset LoadMesh(const char* path, int flags);

    public:
        static Assimp::Importer Importer;

    private:
        static MeshAsset InitScene(const aiScene* scene);

        static Mesh InitMesh(const aiMesh* mesh, const aiScene* scene);
    };
}
