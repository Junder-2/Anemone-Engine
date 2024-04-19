#pragma once
#include <SDL_video.h>

#include "Platform/Vulkan/VulkanRenderer.h"

namespace Engine
{
    struct DrawCommand;
    struct DrawContext;
}

namespace Engine
{

    class Renderer
    {
    public:
        static void Init(SDL_Window* window);
        static void Shutdown();
        static void Render(const WindowProperties& props);

        static VmaMeshAsset LoadModel(const std::string& modelPath);

        static void SubmitDrawCommand(const DrawCommand& command);

        //These methods are sandwiched around the UI Layer render loops
        //ALL Imgui:: name space commands between these two methods will
        //contribute to the next UI command buffer
        static void BeginUiDataBuffer();
        static void EndUIDataBuffer();

        static void SetViewProjection(const Matrix4x4& matrix);

    private:
        static void FlushDrawCommands();

    private:
        static std::unique_ptr<VulkanRenderer> _vulkanRenderer;

        static DrawContext _drawCommands;
    };
}
