#pragma once

#include "ANE/Renderer/RenderingContext.h"

typedef struct SDL_Window SDL_Window;

namespace Vulkan
{
    class VulkanContext : public Engine::RenderingContext
    {
    public:
        VulkanContext(SDL_Window* windowHandle);

        virtual void Init() override;
        virtual void SwapBuffers() override;
        virtual void Cleanup() override;

    private:
        SDL_Window* _windowHandle;
    };
}
