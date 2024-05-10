#include "anepch.h"
#include "DebugRenderer.h"

#include "ANE/Math/Types/Quaternion.h"
#include "Platform/Vulkan/VulkanRenderer.h"

namespace Engine
{
    #ifndef ANE_DIST
    std::vector<Vertex> DebugRenderer::_debugLineVertices;
    std::vector<Vertex> DebugRenderer::_debugTriangleVertices;
    std::vector<DebugDrawCommand> DebugRenderer::_drawCommands;

    Vector4 DebugRenderer::_drawColor = {1, 0, 0, 1};
    Matrix4x4 DebugRenderer::_transformMatrix = Matrix4x4::Identity();
    #endif

    void DebugRenderer::PushColor(const Vector4 color)
    {
        #ifndef ANE_DIST
        _drawColor = color;
        #endif
    }

    void DebugRenderer::PopColor()
    {
        #ifndef ANE_DIST
        _drawColor = {1, 0, 0, 1};
        #endif
    }

    void DebugRenderer::PushTransformMatrix(const Matrix4x4& matrix)
    {
        #ifndef ANE_DIST
        _transformMatrix = matrix;
        #endif
    }

    void DebugRenderer::PopTransformMatrix()
    {
        #ifndef ANE_DIST
        _transformMatrix = Matrix4x4::Identity();
        #endif
    }

    void DebugRenderer::DrawLine(Vector3 start, Vector3 end)
    {
        #ifndef ANE_DIST
        Vertex vert1;
        Vertex vert2;
        vert1.Color = _drawColor;
        vert2.Color = _drawColor;

        if(_transformMatrix == Matrix4x4::Identity())
        {
            vert1.Position = start;
            vert2.Position = end;
        }
        else
        {
            vert1.Position = _transformMatrix * start;
            vert2.Position = _transformMatrix * end;
        }
        PushLine(vert1, vert2);
        #endif
    }

    void DebugRenderer::DrawTriangle(Vector3 point1, Vector3 point2, Vector3 point3)
    {
        #ifndef ANE_DIST
        Vertex vert1;
        Vertex vert2;
        Vertex vert3;
        vert1.Color = _drawColor;
        vert2.Color = _drawColor;
        vert3.Color = _drawColor;

        if(_transformMatrix == Matrix4x4::Identity())
        {
            vert1.Position = point1;
            vert2.Position = point2;
            vert3.Position = point3;
        }
        else
        {
            vert1.Position = _transformMatrix * point1;
            vert2.Position = _transformMatrix * point2;
            vert3.Position = _transformMatrix * point3;
        }
        PushTriangle(vert1, vert2, vert3);
        #endif
    }

    void DebugRenderer::PushLine(const Vertex& vertex1, const Vertex& vertex2)
    {
        #ifndef ANE_DIST
        _debugLineVertices.push_back(vertex1);
        _debugLineVertices.push_back(vertex2);
        #endif
    }

    void DebugRenderer::PushTriangle(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3)
    {
        #ifndef ANE_DIST
        _debugTriangleVertices.push_back(vertex1);
        _debugTriangleVertices.push_back(vertex2);
        _debugTriangleVertices.push_back(vertex3);
        #endif
    }

    VmaMeshBuffers DebugRenderer::UploadDebugMesh(const std::span<uint32_t> indices, const std::span<Vertex> vertices)
    {
        return VulkanRenderer::UploadRawMesh(indices, vertices);
    }

    void DebugRenderer::FetchDrawCommands(std::vector<DebugDrawCommand>& commands)
    {
        #ifndef ANE_DIST
        if(!_debugLineVertices.empty())
        {
            std::vector<uint32_t> indices;
            indices.resize(_debugLineVertices.size());
            for (int i = 0; i < indices.size(); ++i)
            {
                indices[i] = i;
            }

            DebugDrawCommand draw;
            draw.ModelMatrix = Matrix4x4::Identity();
            draw.VertexCount = _debugLineVertices.size();
            draw.MeshBuffers = UploadDebugMesh(indices, _debugLineVertices);
            draw.LineList = true;

            commands.push_back(draw);

            _debugLineVertices.clear();
        }
        if(!_debugTriangleVertices.empty())
        {
            std::vector<uint32_t> indices;
            indices.resize(_debugTriangleVertices.size());
            for (int i = 0; i < indices.size(); ++i)
            {
                indices[i] = i;
            }

            DebugDrawCommand draw;
            draw.ModelMatrix = Matrix4x4::Identity();
            draw.VertexCount = _debugTriangleVertices.size();
            draw.MeshBuffers = UploadDebugMesh(indices, _debugTriangleVertices);
            draw.LineList = false;

            commands.push_back(draw);

            _debugTriangleVertices.clear();
        }
        if(!_drawCommands.empty())
        {
            for (auto drawCommand : _drawCommands)
            {
                commands.push_back(drawCommand);
            }
            _drawCommands.clear();
        }
        #endif
    }
}
