#pragma once
#include <span>
#include "Platform/Vulkan/VmaTypes.h"

namespace Engine
{
    using namespace Vulkan;

    struct Matrix4x4;
    struct Quaternion;
    struct DebugDrawCommand;

    class DebugRenderer
    {
    public:
        static void PushColor(Vector4 color);
        static void PopColor();

        static void PushTransformMatrix(const Matrix4x4& matrix);
        static void PopTransformMatrix();

        static void DrawLine(Vector3 start, Vector3 end);
        static void DrawTriangle(Vector3 point1, Vector3 point2, Vector3 point3);
        //Todo: add Sphere Cube etc

        static void PushLine(const Vertex& vertex1, const Vertex& vertex2);
        static void PushTriangle(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3);

        static VmaMeshBuffers UploadDebugMesh(std::span<uint> indices, std::span<Vertex> vertices);
        static void FetchDrawCommands(std::vector<DebugDrawCommand>& commands);
    private:
        #ifndef ANE_DIST
        static std::vector<Vertex> _debugLineVertices;
        static std::vector<Vertex> _debugTriangleVertices;
        static std::vector<DebugDrawCommand> _drawCommands;

        static Vector4 _drawColor;
        static Matrix4x4 _transformMatrix;
        #endif
    };
}
