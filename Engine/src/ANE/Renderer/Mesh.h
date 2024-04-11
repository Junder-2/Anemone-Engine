#pragma once
#include "Platform/Vulkan/VmaTypes.h"

namespace Engine
{
    struct Mesh
    {
        uint32_t StartIndex;
        uint32_t Count;
    };

    struct MeshAsset
    {
        std::string Name;

        std::vector<Mesh> SubMeshes;
        VmaMeshBuffers MeshBuffers;
    };
}
