#include "anepch.h"
#include "InspectorPanel.h"

#include <memory>
#include "entt.hpp"
#include "imgui.h"


Engine::InspectorPanel::InspectorPanel(std::unordered_map<const char*, std::shared_ptr<Engine::Scene>>& currentScenes)
{
    _managedScenes = &currentScenes;
}

Engine::InspectorPanel::~InspectorPanel()
{
}

void Engine::InspectorPanel::OnPanelRender()
{
    ImGui::Begin("Scene Hierarchy");

    //ANE_LOG_INFO("Entering panel render phase");

    for (auto& it: *_managedScenes) {
        if (ImGui::CollapsingHeader(it.first))
        {
            ImGui::SeparatorText("General");
            for(auto entity: it.second->_registry.view<entt::entity>()) {
                ANE_LOG_INFO("found an entity");
            }
        }

    }

    ImGui::End();
}
