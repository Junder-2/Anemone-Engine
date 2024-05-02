#pragma once
#include <SDL_video.h>

#include "Platform/Vulkan/VulkanRenderer.h"

namespace Engine
{
    struct DrawCommand;
    struct DebugDrawCommand;
    struct DrawContext;

    class Renderer
    {
    public:
        static void Init(SDL_Window* window);
        static void Shutdown();
        static void Render(const WindowProperties& props);

        static VmaMeshAsset LoadModel(const std::string& modelPath);
        static VmaMeshBuffers UploadDebugMesh(std::span<uint32_t> indices, std::span<Vertex> vertices);

        static VmaImage LoadTexture(const std::string& texturePath);

        static void SubmitDrawCommand(const DrawCommand& command);
        static void SubmitDebugDrawCommand(const DebugDrawCommand& command);

        //Should these be here?
        static void DebugDrawLine(const Vertex& vertex1, const Vertex& vertex2);
        static void DebugDrawTriangle(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3);

        //These methods are sandwiched around the UI Layer render loops
        //ALL Imgui:: name space commands between these two methods will
        //contribute to the next UI command buffer
        static void BeginUiDataBuffer();
        static void EndUIDataBuffer();

        static void SetViewProjection(const Matrix4x4& matrix);

        static void SetCameraPosition(Vector3 pos);

    private:
        static void FlushDrawCommands();

    private:
        static std::unique_ptr<VulkanRenderer> _vulkanRenderer;

        static DrawContext _drawCommands;
        //Should these be here?
        static std::vector<Vertex> _debugLineVertices;
        static std::vector<Vertex> _debugTriangleVertices;
    };
}
