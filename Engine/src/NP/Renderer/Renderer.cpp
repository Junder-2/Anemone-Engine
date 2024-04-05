#include "nppch.h"
#include "Renderer.h"

#include <imgui_impl_sdl2.h>

#include "Platform/Vulkan/VulkanRenderer.h"

namespace Engine
{
    void Renderer::Init(SDL_Window* window)
    {
        _vulkanRenderer = std::make_unique<VulkanRenderer>(window);
        //_vulkanRenderer->Setup();
    }

    void Renderer::Shutdown()
    {
       // _vulkanRenderer->Cleanup();
    }

    void Renderer::Render()
    {

    }

    void Renderer::BeginUiDataBuffer()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    void Renderer::EndUIDataBuffer()
    {
        ImGui::Render();
    }
}
