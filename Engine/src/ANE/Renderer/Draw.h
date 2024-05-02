#pragma once
#include "Mesh.h"
#include "ANE/Math/Types/Matrix4x4.h"

namespace Engine
{
    struct DrawCommand
    {
        // TODO: Add other necessary draw command information such as material...
        Matrix4x4 ModelMatrix;
        uint32_t VertexCount;
        VmaMeshBuffers MeshBuffers;
    };

    struct DebugDrawCommand
    {
        Matrix4x4 ModelMatrix;
        uint32_t VertexCount;
        VmaMeshBuffers MeshBuffers;
        bool LineList;
    };

    struct DrawContext
    {
        std::vector<DrawCommand> Commands;
        std::vector<DebugDrawCommand> DebugCommands;
    };
}
