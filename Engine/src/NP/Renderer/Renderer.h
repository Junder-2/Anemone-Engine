#pragma once
#include <SDL_video.h>

#include "Platform/Vulkan/VulkanRenderer.h"

namespace Engine
{

    class Renderer
    {
    public:
        static void Init(SDL_Window* window);
        static void Shutdown();
        static void Render(const WindowProperties& props);

        //These methods are sandwiched around the UI Layer render loops
        //ALL Imgui:: name space commands between these two methods will
        //contribute to the next UI command buffer
        static void BeginUiDataBuffer();
        static void EndUIDataBuffer();

    private:
        static std::unique_ptr<VulkanRenderer> _vulkanRenderer;
    };
}
