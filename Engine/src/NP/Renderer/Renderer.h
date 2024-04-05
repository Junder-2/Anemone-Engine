#pragma once
#include <SDL_video.h>

namespace Engine
{
    class VulkanRenderer;

    class Renderer
    {
    public:
        static void Init(SDL_Window* window);
        static void Shutdown();
        static void Render();

    private:
        static std::unique_ptr<VulkanRenderer> _vulkanRenderer;
    };
}
