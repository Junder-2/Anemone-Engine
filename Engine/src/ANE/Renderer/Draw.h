#pragma once
#include "Mesh.h"
#include "ANE/Core/Math/Matrix/Matrix4x4.h"

namespace Engine
{
    struct DrawCommand
    {
        // TODO: Add other necessary draw command information such as material...
        Matrix4x4 ModelMatrix;
        uint32_t VertexCount;
        VmaMeshBuffers MeshBuffers;
    };

    struct DrawContext
    {
        std::vector<DrawCommand> Commands;
    };
}
