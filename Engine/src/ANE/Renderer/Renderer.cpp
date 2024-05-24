#include "anepch.h"
#include "Renderer.h"

#include <filesystem>
#include <imgui_impl_sdl2.h>

#include "DebugRenderer.h"
#include "Platform/Vulkan/VulkanRenderer.h"
#include "ANE/Math/Types/Matrix4x4.h"
#include "Mesh.h"
#include "Draw.h"
#include "ANE/Events/Event.h"

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
        #ifndef ANE_DIST
        DebugRenderer::FetchDrawCommands(_drawCommands.DebugCommands);
        #endif

        _vulkanRenderer->Render(props, _drawCommands);
        FlushDrawCommands();
    }

    void Renderer::OnEvent(const Event& e)
    {
        if (e.GetEventType() != EventType::WindowResize) return;

        _vulkanRenderer->OnWindowResize();
    }

    VmaMeshAsset Renderer::LoadModel(const std::string& modelPath)
    {
        return _vulkanRenderer->LoadModel(modelPath);
    }

    VmaImage Renderer::LoadTexture(const std::string& texturePath)
    {
        return _vulkanRenderer->LoadTexture(texturePath);
    }

    VmaMeshAsset Renderer::GetDefaultMesh()
    {
        return _vulkanRenderer->GetDefaultMesh();
    }

    MaterialInstance* Renderer::GetDefaultMaterial()
    {
        return _vulkanRenderer->GetDefaultMaterial();
    }

    MaterialInstance* Renderer::GetDefaultMaterialClone()
    {
        return _vulkanRenderer->GetDefaultMaterialClone();
    }

    void Renderer::SubmitDrawCommand(const DrawCommand& command)
    {
        _drawCommands.Commands.push_back(command);
    }

    void Renderer::BeginUIDataBuffer()
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

    void Renderer::SetCameraPosition(const Vector3 pos)
    {
        _vulkanRenderer->CameraPosition = pos;
    }

    void Renderer::SetSunDirection(const Vector3 dir)
    {
        _vulkanRenderer->SunDirection = dir;
    }

    void Renderer::SetSunColor(const Vector3 col)
    {
        _vulkanRenderer->SunColor = col;
    }

    void Renderer::FlushDrawCommands()
    {
        _drawCommands.Commands.clear();
        _drawCommands.DebugCommands.clear();
    }
}
