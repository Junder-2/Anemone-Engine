#include "anepch.h"
#include "SceneHierarchyPanel.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "InspectorPanel.h"
#include "ANE/Core/Editor/SelectionManager.h"
#include "ANE/Core/Scene/Components/TagComponent.h"
#include "ANE/Core/Scene/Components/UUIDComponent.h"


Engine::SceneHierarchyPanel::SceneHierarchyPanel(EditorLayer* ManagingLayer)
{
    _editorLayer = ManagingLayer;
}


void Engine::SceneHierarchyPanel::OnPanelRender()
{
    _activeScene = _editorLayer->GetActiveScene();
    if(!_activeScene)
    {
        return;
    }
    bool open = true;

    ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

    ImGui::Begin("Scene Hierarchy",&open,dockspace_flags);

    DrawEntityNodeList();

    ImGui::End();
}



void Engine::SceneHierarchyPanel::DrawEntityNodeList()
{
    std::vector<std::string>* selectedEntityUUIDS = SelectionManager::GetSelection(SelectionManager::UI);
    auto IDView = _activeScene->_registry.view<UUIDComponent,TagComponent>();
    for(auto entity: IDView)
    {
        auto &UUID = IDView.get<UUIDComponent>(entity);
        auto &Tag = IDView.get<TagComponent>(entity);
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

        if(std::find(selectedEntityUUIDS->begin(), selectedEntityUUIDS->end(), UUID.UUID) != selectedEntityUUIDS->end()){
            node_flags |= ImGuiTreeNodeFlags_Selected;
        }
        DrawEntityNode(UUID,Tag,node_flags);

    }

}

void Engine::SceneHierarchyPanel::DrawEntityNode(UUIDComponent UUID, TagComponent Tag, ImGuiTreeNodeFlags node_flags)
{

    bool node_open = ImGui::TreeNodeEx(UUID.UUID.c_str(), node_flags, Tag.Tag.c_str());

    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
    {
        SelectionManager::DeSelect(_SelectionContext);
        SelectionManager::RegisterSelect(_SelectionContext,UUID.UUID);
    }
    if(node_open)
    {
        //If heirarchy present, do recursive draw here
        ImGui::TreePop();
    }
}





