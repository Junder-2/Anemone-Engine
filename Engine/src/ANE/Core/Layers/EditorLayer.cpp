#include "anepch.h"
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
#include "Panels/MainMenuPanel.h"
#include "Panels/CreateScenePanel.h"
#include "Panels/UIUpdateWrapper.h"

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
        AttachUIPanel(new SceneHierarchyPanel(this));
        AttachUIPanel(new InspectorPanel(this));
        AttachUIPanel(new EditorLogPanel(this));
        AttachUIPanel(new MainMenuPanel(this));

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
            if (panel == nullptr) continue;

            if (panel->IsVisible())
            {
                auto update = panel->OnPanelRender();

                UIUpdates.emplace(UIUpdates.begin(), update);
            }

            // if(panel->IsEnabled())
            // {
            //     //panel->doWindowMaintenance
            // }
        }


        for (auto UIUpdate : UIUpdates)
        {
            if (UIUpdate.RemoveSelf != nullptr) DetachUIPanel(UIUpdate.RemoveSelf);

            for (auto panel : UIUpdate.PanelsToAdd) AddPanel(panel);

            UIUpdate.Clean();
        }

        UIUpdates.clear();

        ImGui::Begin("Editor");
        ImGui::End();
    }

    void EditorLayer::Init()
    {
        TagComponent::RegisterComponentMetaData();
        TransformComponent::RegisterComponentMetaData();
        ColliderComponent::RegisterComponentMetaData();
        RigidBodyComponent::RegisterComponentMetaData();
        RenderComponent::RegisterComponentMetaData();
        UUIDComponent::RegisterComponentMetaData();
        CameraComponent::RegisterComponentMetaData();
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
        Entity ent = Create("Square Entity");
        std::stringstream oss;

        for (int i = 0; i < numEntitiesToTest; i++)
        {
            //ANE_LOG_INFO(UUIDGenerator::GetUUID());

            std::string string = "Entity";
            string.append(std::to_string(i));
            string.append("\n");
            Create(string);
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
        Entity floor = Create("Floor");
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

        if (inputValue.GetDeviceType() == InputDeviceKeyboard)
        {
            if (inputValue.GetTriggerState() != TriggerStarted || blockingAppInputs) return;
            switch (inputValue.GetBindingId())
            {
                case KeyCodeEscape:
                    ShowMouse();
                    blockingAppInputs = true;
                    break;
                default: return;
            }
        }
        else if (inputValue.GetDeviceType() == InputDeviceMouse) //Any mouse click should return focus
        {
            //Reject any refocus it is not a doubleclick or if the mouse is not over viewport
            if (inputValue.GetTriggerState() != TriggerStarted || !blockingAppInputs) return;
            if (!GetInputSystem().GetMouseButtonValues().GetIsDoubleClick()) return;
            if (!Application::Get().GetWindow().IsOverViewport()) return;

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

        if (reg.valid(e))
        {
            ImGui::Text("ID: %d", entt::to_integral(e));
        }
        else
        {
            ImGui::Text("Invalid Entity");
        }

        if (reg.valid(e))
        {
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
            {
                ImGui::SetDragDropPayload(MM_IEEE_IMGUI_PAYLOAD_TYPE_ENTITY, &e, sizeof(e));
                ImGui::Text("ID: %d", entt::to_integral(e));
                ImGui::EndDragDropSource();
            }
        }

        if (dropTarget && ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(MM_IEEE_IMGUI_PAYLOAD_TYPE_ENTITY))
            {
                e = *(EntityType*)payload->Data;
            }

            ImGui::EndDragDropTarget();
        }

        ImGui::PopID();
    }


    /**
    * \brief Creates a Entity in the scene, adds a Transform and Tag
    * \param name Name of Entity, if no name is given it will be tagged with: "Untagged"
    * \return reference of the newly created Entity.
    */
    [[nodiscard("Entity never used")]] Entity EditorLayer::Create(const char* name)
    {
        Entity ent{GetActiveScene().get(), name};
        _entityMap[ent.GetComponent<UUIDComponent>().UUID] = ent; // here
        return ent;
    }

    [[nodiscard("Entity never used")]] Entity EditorLayer::Create(std::string stringName)
    {
        Entity ent{GetActiveScene().get(), stringName.c_str()};
        _entityMap[ent.GetComponent<UUIDComponent>().UUID] = ent;
        return ent;
    }

    Entity EditorLayer::GetEntityWithUUID(std::string UUID)
    {
        return _entityMap[UUID];
        return {};
    }
}
