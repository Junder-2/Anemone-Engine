#pragma once
#include <SDL_video.h>

#include "Platform/Vulkan/VulkanRenderer.h"

namespace Engine
{
    class Event;
    struct DrawCommand;
    struct DrawContext;

    class Renderer
    {
    public:
        static void Init(SDL_Window* window);
        static void Shutdown();
        static void Render(const WindowProperties& props);

        static void OnEvent(const Event& e);

        static VmaMeshAsset LoadModel(const std::string& modelPath);

        static Vulkan::VmaImage LoadTexture(const std::string& texturePath);

        static void SubmitDrawCommand(const DrawCommand& command);

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
        static std::unique_ptr<Vulkan::VulkanRenderer> _vulkanRenderer;

        static DrawContext _drawCommands;
    };
}
