﻿#include "anepch.h"
#include "EditorLayer.h"
#include <imgui.h>

#include "ANE/Core/Entity/Entity.h"
#include "LayerStack.h"
#include "ANE/Events/EventHandler.h"

//Temp includes. Can probably change how these components are referenced. Maybe a scene manager hsould be in charge of that
//kind of thing
#include "ANE/Core/Entity/ExampleScripts/CameraController.h"
#include "ANE/Core/Scene/Components/NativeScriptComponent.h"
#include "ANE/Core/Scene/Components/RenderComponent.h"
#include "Panels/InspectorPanel.h"
#include "Panels/SceneHierarchyPanel.h"

namespace Engine
{

    #ifndef MM_IEEE_ASSERT
    #define MM_IEEE_ASSERT(x) assert(x)
    #endif

    #define MM_IEEE_IMGUI_PAYLOAD_TYPE_ENTITY "MM_IEEE_ENTITY"

    #ifndef MM_IEEE_ENTITY_WIDGET
    #define MM_IEEE_ENTITY_WIDGET ::MM::EntityWidget
    #endif

    EditorLayer::EditorLayer(const std::string& name) : Layer(name)
    {
        Init();
    }

    EditorLayer::~EditorLayer() = default;

    void EditorLayer::OnAttach()
    {

        // You would have a "Read from config files to find correct panel layout" method here

        CreateTestScene(50);
        AttachUIPanel(new SceneHierarchyPanel(_scenes));
        AttachUIPanel(new InspectorPanel(this));

        // Then you would call load methods to load the most recent project

        //Then you would load the scene from the file path listed from that project
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnEvent(Event& e)
    {
        EventHandler::DispatchEditorEvents();
        EventHandler::DispatchAppEvents();
    }

    void EditorLayer::OnUIRender()
    {
        for (UILayerPanel* panel : _UIpanels)
        {
            if(panel->_isVisible)
            {
                panel->OnPanelRender();
            }
        ImGui::Begin("Hello");
        if(ImGui::Button("Button"))
        {
            Entity ent = GetActiveScene()->Create("Square Entity");

        }
        ImGui::End();

            /*There is a chance we will have situation where UI is not visible but still needs to do something
            like maintaining a dockspace or something like that. This loop is for that situation.
            If it remains empty for ages we can delete it but leaving it HEAP_REALLOC_IN_PLACE_ONLY*/
            if(panel->_isEnabled)
            {
                //panel->doWindowLayoutmaitenance
            }
        }
        static bool showSimpleOverlay = true;
        if (showSimpleOverlay) ShowInputDebugOverlay(&showSimpleOverlay);

        ImGui::ShowDemoWindow();
    }

    void EditorLayer::Init()
    {
        //Every component type needs to be in here in order for it's text to be rendered
        //Ideally, Kyle will develop this into component type specific renderering methods
        //and we can then delete this map because it's kind of stupid, it's just quick and dirty
        ComponentTypeMap[entt::type_id<TagComponent>().hash()] = "TagComponent";
        ComponentTypeMap[entt::type_id<TransformComponent>().hash()] = "TransformComponent";
        ComponentTypeMap[entt::type_id<NativeScriptComponent>().hash()] = "NativeScriptComponent";
        ComponentTypeMap[entt::type_id<UUIDComponent>().hash()] = "UUIDComponent";
        ComponentTypeMap[entt::type_id<RenderComponent>().hash()] = "RenderComponent";
    }

    void EditorLayer::OnUpdate(float deltaTime)
    {
        if (_activeScene) _activeScene->OnUpdate(deltaTime);
    }

    std::string EditorLayer::GetComponentNameFromEnttId(entt::id_type id)
    {
        return ComponentTypeMap[id];
    }

    void EditorLayer::CreateTestScene(int numEntitiesToTest)
    {
        //Add scene to layer
        AddScene<Scene>("Game");

        //Create a Entity
        Entity ent = GetActiveScene()->Create("Square Entity");
        std::stringstream oss;

        for(int i = 0; i < numEntitiesToTest;i++ )
        {
            //ANE_LOG_INFO(UUIDGenerator::get_uuid());

            std::string string = "Entity";
            string.append(std::to_string(i));
            string.append("\n");
            _activeScene->Create(string);
        }
        //Add component to entity
        ent.AddComponent<RenderComponent>();
        // ent.AddComponent<NativeScriptComponent>().Bind<CameraController>();

        //Get Component from entity
        if (RenderComponent comp; ent.TryGetComponent<RenderComponent>(comp))
        {
            TagComponent tag;
            ent.TryGetComponent(tag);
            ANE_ENGINE_LOG_WARN("We have a renderComponent with tag: {0} on entity: {1}", comp.ToString(), tag.Tag);
        }
    }

    //Copied from IMGUI example
    void EditorLayer::ShowInputDebugOverlay(bool* pOpen)
    {
        const InputSystem& inputManager = GetInputSystem();
        static int location = 0;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDocking;
        if (location >= 0)
        {
            const float PAD = 10.0f;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
            ImVec2 work_size = viewport->WorkSize;
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
            window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
            window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
            window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            window_flags |= ImGuiWindowFlags_NoMove;
        }
        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        if (ImGui::Begin("Example: Simple overlay", pOpen, window_flags))
        {
            const glm::vec2 mousePos = inputManager.GetMousePos();
            ImGui::Text("Mouse Pos: (%.3f,%.3f)", mousePos.x, mousePos.y);
            const MouseButtonValues mouseButtonValues = inputManager.GetMouseButtonValues();
            ImGui::Text("Mouse Buttons: (");
            bool start = true;
            for (int i = 0; i < MOUSE_BUTTON_MAX; i++)
            {
                const int buttonState = mouseButtonValues.GetTriggerState(i);
                if (buttonState == 0) continue;
                ImGui::SameLine(0, start ? .0f : -1.0f);
                ImGui::Text("%d:%d", i, buttonState);
                start = false;
            }
            ImGui::SameLine(0, 0);
            ImGui::Text(")");

            const auto keyValues = inputManager.GetCurrentTriggeredKeys();
            ImGui::Text("Keyboard: (");
            start = true;
            for (auto keyValue : keyValues)
            {
                if(keyValue.GetIntValue() == 0) continue;
                ImGui::SameLine(0, start ? .0f : -1.0f);
                ImGui::Text("%d:%d", keyValue.GetBindingId(), keyValue.GetIntValue());
                start = false;
            }
            ImGui::SameLine(0, 0);
            ImGui::Text(")");

            if (ImGui::BeginPopupContextWindow())
            {
                if (pOpen && ImGui::MenuItem("Close")) *pOpen = false;
                ImGui::EndPopup();
            }
        }
        ImGui::End();
    }

    template <class EntityType>
    void EditorLayer::EntityWidget(EntityType& e, entt::basic_registry<EntityType>& reg, bool dropTarget)
    {
        ImGui::PushID(static_cast<int>(entt::to_integral(e)));

        if (reg.valid(e)) {
            ImGui::Text("ID: %d", entt::to_integral(e));
        } else {
            ImGui::Text("Invalid Entity");
        }

        if (reg.valid(e)) {
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
                ImGui::SetDragDropPayload(MM_IEEE_IMGUI_PAYLOAD_TYPE_ENTITY, &e, sizeof(e));
                ImGui::Text("ID: %d", entt::to_integral(e));
                ImGui::EndDragDropSource();
            }
        }

        if (dropTarget && ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(MM_IEEE_IMGUI_PAYLOAD_TYPE_ENTITY)) {
                e = *(EntityType*)payload->Data;
            }

            ImGui::EndDragDropTarget();
        }

        ImGui::PopID();
    }

}
