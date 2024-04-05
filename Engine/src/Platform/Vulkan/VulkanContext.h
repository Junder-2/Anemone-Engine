#pragma once

#include "ANE/Renderer/RenderingContext.h"

typedef struct SDL_Window SDL_Window;

namespace Engine
{
    class VulkanContext : public RenderingContext
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
