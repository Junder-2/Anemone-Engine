#include "anepch.h"
#include "Renderer.h"

#include <filesystem>
#include <imgui_impl_sdl2.h>

#include "Platform/Vulkan/VulkanRenderer.h"
#include "ANE/Math/Types/Matrix4x4.h"
#include "Mesh.h"
#include "Draw.h"

namespace Engine
{
    std::unique_ptr<VulkanRenderer> Renderer::_vulkanRenderer;
    DrawContext Renderer::_drawCommands;

    void Renderer::Init(SDL_Window* window)
    {
        _vulkanRenderer = std::make_unique<VulkanRenderer>(window);
        _vulkanRenderer->Setup();
    }

    void Renderer::Shutdown()
    {
       _vulkanRenderer->Cleanup();
    }

    void Renderer::Render(const WindowProperties& props)
    {
        _vulkanRenderer->Render(props, _drawCommands);
        FlushDrawCommands();
    }

    VmaMeshAsset Renderer::LoadModel(const std::string& modelPath)
    {
        return _vulkanRenderer->LoadModel(modelPath);
    }

    VmaImage Renderer::LoadTexture(const std::string& texturePath)
    {
        return _vulkanRenderer->LoadTexture(texturePath);
    }

    void Renderer::SubmitDrawCommand(const DrawCommand& command)
    {
        _drawCommands.Commands.push_back(command);
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

        if (_vulkanRenderer->GetImGuiIO()->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    void Renderer::SetViewProjection(const Matrix4x4& matrix)
    {
        _vulkanRenderer->ViewProjection = matrix;
    }

    void Renderer::FlushDrawCommands()
    {
        _drawCommands.Commands.clear();
    }
}
