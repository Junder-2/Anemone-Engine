#pragma once
#include "Platform/Vulkan/VmaTypes.h"

namespace Engine
{
    struct Mesh
    {
        uint NumIndices;
        uint Material;
        uint StartOffset;
        std::vector<Vulkan::Vertex> Vertices;
        std::vector<unsigned int> Indices;
    };

    struct MeshAsset
    {
        uint NumVertices;
        std::vector<Mesh> SubMeshes;
    };

    struct VmaMeshAsset
    {
        std::string Name;

        uint NumVertices;
        std::vector<Mesh> SubMeshes;
        Vulkan::VmaMeshBuffers MeshBuffers;
    };
}
