#include "anepch.h"
#include "ViewportPanel.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "ANE/Core/Application.h"
#include "ANE/Core/Window.h"
#include "Platform/Vulkan/VulkanRenderer.h"

namespace Engine
{
    ViewportPanel::ViewportPanel(EditorLayer* editorLayer)
    {
        _editorLayer = editorLayer;
    }

    ViewportPanel::~ViewportPanel()
    {
        Application::Get().GetWindow().RemoveViewport(ImHashStr("Viewport"));
    }

    UIUpdateWrapper ViewportPanel::OnPanelRender()
    {
        UIUpdateWrapper uiUpdate;

        ImGui::SetNextWindowSizeConstraints(Vector2(100.f, 100.f), Vector2(FLT_MAX, FLT_MAX));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
        if (ImGui::Begin("Viewport"))
        {
            if(!_initialized)
            {
                Application::Get().GetWindow().SetActiveViewport(ImGui::GetCurrentWindow()->ID);
                _initialized = false;
            }
            ImGui::Image(VulkanRenderer::GetImGuiViewportSet(), ImGui::GetContentRegionAvail());
        }
        ImGui::End();
        ImGui::PopStyleVar();

        return uiUpdate;
    }
}
