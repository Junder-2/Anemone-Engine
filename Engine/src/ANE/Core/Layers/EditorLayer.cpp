﻿#include "anepch.h"
#include "EditorLayer.h"
#include <imgui.h>

#include "ANE/Core/Entity/Entity.h"
#include "LayerStack.h"
#include "ANE/Events/EventHandler.h"

//Temp includes. Can probably change how these components are referenced. Maybe a scene manager hsould be in charge of that
//kind of thing
#include "imgui_internal.h"
#include "ANE/Core/Application.h"
#include "ANE/Core/Window.h"
#include "ANE/Core/Entity/ExampleScripts/CameraController.h"
#include "ANE/Core/Scene/Components/CameraComponent.h"
#include "ANE/Core/Scene/Components/ColliderComponent.h"
#include "ANE/Core/Scene/Components/NativeScriptComponent.h"
#include "ANE/Core/Scene/Components/RenderComponent.h"
#include "ANE/Core/Scene/Components/RigidBodyComponent.h"
#include "Panels/InspectorPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "ANE/Input/EditorInputSystem.h"
#include "ANE/Input/Input.h"
#include "ANE/Input/InputAction.h"
#include "Panels/EditorLogPanel.h"

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
        TagComponent::RegisterComponentMetaData();
        TransformComponent::RegisterComponentMetaData();
        ColliderComponent::RegisterComponentMetaData();
        CreateTestScene(50);
        AttachUIPanel(new SceneHierarchyPanel(this));
        AttachUIPanel(new InspectorPanel(this));
        AttachUIPanel(new EditorLogPanel(this));

        // Then you would call load methods to load the most recent project

        //Then you would load the scene from the file path listed from that project

        GetEditorInputSystem().BindKeyboardInput(KeyCodeEscape, MakeDelegate(this, &EditorLayer::OnSwitchEditorFocus));
        GetEditorInputSystem().BindMouseButton(MouseButtonLeft, MakeDelegate(this, &EditorLayer::OnSwitchEditorFocus));
        GetEditorInputSystem().BindMouseButton(MouseButtonRight, MakeDelegate(this, &EditorLayer::OnSwitchEditorFocus));
        GetEditorInputSystem().BindMouseButton(MouseButtonMiddle, MakeDelegate(this, &EditorLayer::OnSwitchEditorFocus));

        //TEMP need to set the initial state should be elsewhere
        EventHandler::SetBlockAppInputs(true);
    }

    void EditorLayer::OnDetach()
    {

    }

    void EditorLayer::OnEvent(Event& e)
    {
        Layer::OnEvent(e);

        EventHandler::DispatchEditorEvents();
        EventHandler::DispatchAppEvents();
    }

    void EditorLayer::OnUIRender()
    {
        ImGuiIO& io = ImGui::GetIO();

        for (UILayerPanel* panel : _UIpanels)
        {
            if(panel->IsVisible())
            {
                panel->OnPanelRender();
            }

            if(panel->IsEnabled())
            {
                //panel->doWindowMaintenance
            }
        }
        ImGui::Begin("Editor");
        ImGui::End();
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
        ComponentTypeMap[entt::type_id<RigidBodyComponent>().hash()] = "RigidBodyComponent";
        ComponentTypeMap[entt::type_id<ColliderComponent>().hash()] = "ColliderComponent";
    }

    void EditorLayer::OnUpdate(float deltaTime)
    {
        Layer::OnUpdate(deltaTime);

        if (_activeScene) _activeScene->OnUpdate(deltaTime);
    }

    std::string EditorLayer::GetComponentNameFromEnttId(const entt::id_type id)
    {
        return ComponentTypeMap[id];
    }

    void EditorLayer::CreateTestScene(int numEntitiesToTest)
    {
        ANE_PROFILE_FUNCTION();

        //Add scene to layer
        AddScene<Scene>("Game");

        //Create a Entity
        Entity ent = GetActiveScene()->Create("Square Entity");
        std::stringstream oss;

        for(int i = 0; i < numEntitiesToTest;i++ )
        {
            //ANE_LOG_INFO(UUIDGenerator::GetUUID());

            std::string string = "Entity";
            string.append(std::to_string(i));
            string.append("\n");
            GetActiveScene()->Create(string);
        }
        //Add component to entity
        //ent.AddComponent<RenderComponent>();
        ent.AddComponent<CameraComponent>();
        ent.AddComponent<NativeScriptComponent>().Bind<CameraController>();

        CreateFloor();

        //Get Component from entity
        if (RenderComponent comp; ent.TryGetComponent<RenderComponent>(comp))
        {
            TagComponent tag;
            ent.TryGetComponent(tag);
            ANE_ELOG_WARN("We have a renderComponent with tag: {0} on entity: {1}", comp.ToString(), tag.Value);
        }
    }

    void EditorLayer::CreateFloor()
    {
        Entity floor = GetActiveScene()->Create("Floor");
        TransformMatrix& transformMatrix = floor.GetComponent<TransformComponent>().Transform;
        transformMatrix.SetPosition(Vector3(0, -5.f, 0));
        transformMatrix.Scale(Vector3(10.f));

        floor.AddComponent<RenderComponent>("Plane.obj");
        floor.AddComponent<RigidBodyComponent>(floor, BodyType::Static);
        floor.AddComponent<ColliderComponent>(floor, Vector3(10.f, .1f, 10.f));
    }

    void EditorLayer::OnSwitchEditorFocus(InputValue inputValue)
    {
        bool blockingAppInputs = IsMouseVisible();

        if(inputValue.GetDeviceType() == InputDeviceKeyboard)
        {
            if(inputValue.GetTriggerState() != TriggerStarted || blockingAppInputs) return;
            switch (inputValue.GetBindingId())
            {
                case KeyCodeEscape:
                    ShowMouse();
                    blockingAppInputs = true;
                break;
                default: return;
            }
        }
        else if(inputValue.GetDeviceType() == InputDeviceMouse) //Any mouse click should return focus
        {
            //Reject any refocus it is not a doubleclick or if the mouse is not over viewport
            if(inputValue.GetTriggerState() != TriggerStarted || !blockingAppInputs) return;
            if(!GetInputSystem().GetMouseButtonValues().GetIsDoubleClick()) return;
            if(!Application::Get().GetWindow().IsOverViewport()) return;

            HideMouse();
            blockingAppInputs = false;
        }

        EventHandler::SetBlockAppInputs(blockingAppInputs);
        EventHandler::ConsumeEvent();
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

    template <class TValue, typename... Args>
    std::enable_if_t<std::is_base_of_v<UILayerPanel, TValue>> EditorLayer::AddPanel(Args&&... args)
    {
        Layer::AttachUIPanel(new TValue(std::forward<Args>(args)...));
    }
}
