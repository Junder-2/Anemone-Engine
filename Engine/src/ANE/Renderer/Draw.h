#pragma once
#include "Mesh.h"
#include "ANE/Math/Types/Matrix4x4.h"
#include "Platform/Vulkan/VulkanMaterial.h"

namespace Engine
{
    struct DrawCommand
    {
        Vulkan::MaterialInstance* Material;
        Matrix4x4 ModelMatrix;
        uint32_t VertexCount;
        Vulkan::VmaMeshBuffers MeshBuffers;
    };

    struct DebugDrawCommand
    {
        Matrix4x4 ModelMatrix;
        uint32_t VertexCount;
        Vulkan::VmaMeshBuffers MeshBuffers;
        bool LineList;
    };

    struct DrawContext
    {
        std::vector<DrawCommand> Commands;
        std::vector<DebugDrawCommand> DebugCommands;
    };
}
