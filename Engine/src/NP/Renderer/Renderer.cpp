#include "nppch.h"
#include "Renderer.h"

#include "../../Platform/Vulkan/VulkanRenderer.h"

namespace Engine
{
    void Renderer::Init(SDL_Window* window)
    {
        _vulkanRenderer = std::make_unique<VulkanRenderer>(window);
        _vulkanRenderer->Setup();
    }

    void Renderer::Shutdown()
    {
        _vulkanRenderer->Cleanup();
    }

    void Renderer::Render()
    {
        
    }
}
