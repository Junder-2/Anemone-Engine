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
    std::vector<Vertex> Renderer::_debugLineVertices;
    std::vector<Vertex> Renderer::_debugTriangleVertices;

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
        if(!_debugLineVertices.empty())
        {
            std::vector<uint32_t> indices;
            indices.resize(_debugLineVertices.size());
            for (int i = 0; i < indices.size(); ++i)
            {
                indices[i] = i;
            }

            DebugDrawCommand draw;
            draw.ModelMatrix = Matrix4x4::Identity();
            draw.VertexCount = _debugLineVertices.size();
            draw.MeshBuffers = UploadDebugMesh(indices, _debugLineVertices);
            draw.LineList = true;

            SubmitDebugDrawCommand(draw);

            _debugLineVertices.clear();
        }
        if(!_debugTriangleVertices.empty())
        {
            std::vector<uint32_t> indices;
            indices.resize(_debugTriangleVertices.size());
            for (int i = 0; i < indices.size(); ++i)
            {
                indices[i] = i;
            }

            DebugDrawCommand draw;
            draw.ModelMatrix = Matrix4x4::Identity();
            draw.VertexCount = _debugTriangleVertices.size();
            draw.MeshBuffers = UploadDebugMesh(indices, _debugTriangleVertices);
            draw.LineList = false;

            SubmitDebugDrawCommand(draw);

            _debugTriangleVertices.clear();
        }
        #endif

        _vulkanRenderer->Render(props, _drawCommands);
        FlushDrawCommands();
    }

    VmaMeshAsset Renderer::LoadModel(const std::string& modelPath)
    {
        return _vulkanRenderer->LoadModel(modelPath);
    }

    VmaMeshBuffers Renderer::UploadDebugMesh(const std::span<uint32_t> indices, const std::span<Vertex> vertices)
    {
        return _vulkanRenderer->UploadDebugVertices(indices, vertices);
    }

    VmaImage Renderer::LoadTexture(const std::string& texturePath)
    {
        return _vulkanRenderer->LoadTexture(texturePath);
    }

    void Renderer::SubmitDrawCommand(const DrawCommand& command)
    {
        _drawCommands.Commands.push_back(command);
    }

    void Renderer::SubmitDebugDrawCommand(const DebugDrawCommand& command)
    {
        _drawCommands.DebugCommands.push_back(command);
    }

    void Renderer::DebugDrawLine(const Vertex& vertex1, const Vertex& vertex2)
    {
        #ifdef ANE_DIST
        return;
        #endif
        _debugLineVertices.push_back(vertex1);
        _debugLineVertices.push_back(vertex2);
    }

    void Renderer::DebugDrawTriangle(const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3)
    {
        #ifdef ANE_DIST
        return;
        #endif
        _debugTriangleVertices.push_back(vertex1);
        _debugTriangleVertices.push_back(vertex2);
        _debugTriangleVertices.push_back(vertex3);
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

    void Renderer::SetCameraPosition(const Vector3 pos)
    {
        _vulkanRenderer->CameraPosition = pos;
    }

    void Renderer::FlushDrawCommands()
    {
        _drawCommands.Commands.clear();
        _drawCommands.DebugCommands.clear();
    }
}
