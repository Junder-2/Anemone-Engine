#pragma once
#include "Platform/Vulkan/VmaTypes.h"

namespace Engine
{
    struct Mesh
    {
        uint32_t NumIndices;
        uint32_t Material;
        uint32_t StartOffset;
        std::vector<Vertex> Vertices;
        std::vector<unsigned int> Indices;
    };

    struct MeshAsset
    {
        uint32_t NumVertices;
        std::vector<Mesh> SubMeshes;
    };

    struct VmaMeshAsset
    {
        std::string Name;

        uint32_t NumVertices;
        std::vector<Mesh> SubMeshes;
        VmaMeshBuffers MeshBuffers;
    };
}
