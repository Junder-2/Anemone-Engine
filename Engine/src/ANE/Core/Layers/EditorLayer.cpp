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
#include "ANE/Core/Scene/Components/Components.h"
#include "Panels/InspectorPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "ANE/Input/EditorInputSystem.h"
#include "ANE/Input/Input.h"
#include "ANE/Input/InputAction.h"
#include "ANE/Math/Types/TransformMatrix.h"
#include "ANE/Physics/Physics.h"
#include "ANE/Utilities/SceneSerializer.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/EditorLogPanel.h"
#include "Panels/MainMenuPanel.h"
#include "Panels/UIUpdateWrapper.h"
#include "Panels/ViewportPanel.h"

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
        ANE_PROFILE_FUNCTION();

        Init();
    }

    EditorLayer::~EditorLayer() = default;


    void EditorLayer::Init()
    {
        TagComponent::RegisterComponentMetaData();
        TransformComponent::RegisterComponentMetaData();
        ColliderComponent::RegisterComponentMetaData();
        RigidBodyComponent::RegisterComponentMetaData();
        RenderComponent::RegisterComponentMetaData();
        UUIDComponent::RegisterComponentMetaData();
        CameraComponent::RegisterComponentMetaData();
        NativeScriptComponent::RegisterComponentMetaData();
        LightComponent::RegisterComponentMetaData();

        _sceneSerializer = new SceneSerializer();
    }

    void EditorLayer::OnAttach()
    {
        ANE_PROFILE_FUNCTION();

        // You would have a "Read from config files to find correct panel layout" method here

        CreateTestScene(50);
        AttachUIPanel(new ViewportPanel(this));
        AttachUIPanel(new ContentBrowserPanel(this));
        AttachUIPanel(new SceneHierarchyPanel(this));
        AttachUIPanel(new InspectorPanel(this));
        AttachUIPanel(new EditorLogPanel(this));
        AttachUIPanel(new MainMenuPanel(this));

        // Then you would call load methods to load the most recent project

        //Then you would load the scene from the file path listed from that project

        //GetEditorInputSystem().BindKeyboardInput(KeyCodeO, MakeDelegate(this, &EditorLayer::SaveScene));
        GetEditorInputSystem().BindMouseButton(MouseButtonRight, MakeDelegate(this, &EditorLayer::OnSwitchEditorFocus));

        //TEMP need to set the initial state should be elsewhere
        EventHandler::SetBlockAppInputs(true);
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnEvent(Event& e)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        EventHandler::DispatchEditorEvents();
        EventHandler::DispatchAppEvents();
    }

    void EditorLayer::OnUIRender()
    {
        ANE_DEEP_PROFILE_FUNCTION();

        ImGuiIO& io = ImGui::GetIO();
        ImGui::ShowDemoWindow();
        for (UILayerPanel* panel : _uiPanels)
        {
            if (panel == nullptr) continue;

            if (panel->IsVisible())
            {
                auto update = panel->OnPanelRender();

                _uiUpdates.emplace(_uiUpdates.begin(), update);
            }

            // if(panel->IsEnabled())
            // {
            //     //panel->doWindowMaintenance
            // }
        }


        for (auto UIUpdate : _uiUpdates)
        {
            if (UIUpdate.RemoveSelf != nullptr) DetachUIPanel(UIUpdate.RemoveSelf);

            for (auto panel : UIUpdate.PanelsToAdd) AddPanel(panel);

            UIUpdate.Clean();
        }

        _uiUpdates.clear();
    }

    void EditorLayer::CreateEmptyEntity()
    {
        _activeScene->Create("");
    }

    void EditorLayer::OnUpdate(float deltaTime)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        if (_activeScene) _activeScene->OnUpdate(deltaTime);
    }

    std::string EditorLayer::GetComponentNameFromEnttId(const entt::id_type id)
    {
        return _componentTypeMap[id];
    }

    void EditorLayer::SetActiveScene(const char* sceneName)
    {
        std::shared_ptr<Scene> scene = nullptr;
        if (_activeScene == nullptr) scene = CreateScene(sceneName);
        else scene = _sceneSerializer->Deserialize(sceneName, this);

        ANE_ASSERT(scene == nullptr, "Scene with name: {} does not exist", sceneName);

        _activeScene = scene;
    }

    void EditorLayer::CreateTestScene(int numEntitiesToTest) // this should be remade and be replaced with default scene
    {
        ANE_PROFILE_FUNCTION();

        if (!_sceneSerializer->HasFile("Game")) _activeScene = _sceneSerializer->CreateEmptySceneFile("Game");
        else
        {
            _activeScene = _sceneSerializer->Deserialize("Game", this);
            return;
        }

        //Create a Entity
        Entity ent = _activeScene->Create("Camera");

        for (int i = 0; i < 10; i++)
        {
            std::string entityName = "Entity";
            entityName.append(std::to_string(i));
            _activeScene->Create(entityName);
        }
        //Add component to entity
        //ent.AddComponent<RenderComponent>();
        ent.AddComponent<CameraComponent>();
        ent.AddComponent<NativeScriptComponent>().Bind<CameraController>();

        CreateDefaultFloor();
        CreateDefaultLights();

        //Get Component from entity
        if (RenderComponent comp; ent.TryGetComponent<RenderComponent>(comp))
        {
            TagComponent tag;
            ent.TryGetComponent(tag);
            ANE_ELOG_WARN("We have a renderComponent with tag: {0} on entity: {1}", comp.ToString(), tag.Value);
        }

        _sceneSerializer->Serialize(_activeScene);
    }

    void EditorLayer::CreateDefaultFloor()
    {
        ANE_PROFILE_FUNCTION();

        Entity floor = _activeScene->Create("Floor");
        TransformMatrix& transformMatrix = floor.GetComponent<TransformComponent>().Transform;
        transformMatrix.SetPosition(Vector3(0, -5.f, 0));
        transformMatrix.Scale(Vector3(10.f, 1.f, 10.f));

        floor.AddComponent<RenderComponent>("Plane.obj");
        floor.AddComponent<RigidBodyComponent>(floor, BodyType::Static);
        floor.AddComponent<ColliderComponent>(floor, Vector3(1.f, .1f, 1.f));
    }

    void EditorLayer::CreateDefaultLights()
    {
        ANE_PROFILE_FUNCTION();

        Vector3 ambientColor = Vector3(0.05f, 0.08f, 0.11f);
        const Vector3 sunAngle = Vector3(50 * FMath::DEGREES_TO_RAD, -30 * FMath::DEGREES_TO_RAD, 0);
        const Vector3 sunColor = Vector3(1.0f, 0.9f, 0.67f);

        Entity skyLight = _activeScene->Create("Skylight");
        TransformMatrix& transformMatrix = skyLight.GetComponent<TransformComponent>().Transform;
        transformMatrix.SetRotation(sunAngle);

        LightComponent& lightComp = skyLight.AddComponent<LightComponent>();

        lightComp.SetColor(sunColor);
        lightComp.SetIntensity(1.f);
    }

    void EditorLayer::OnSwitchEditorFocus(const InputValue inputValue)
    {
        bool blockingAppInputs = IsMouseVisible();
        if (inputValue.GetDeviceType() != InputDeviceMouse) return;

        const TriggerState triggerState = inputValue.GetTriggerState();

        if (triggerState == TriggerStarted && blockingAppInputs && Application::Get().GetWindow().IsOverViewport())
        {
            // Return if user might be performing another action
            if(ImGui::IsMouseDown(ImGuiMouseButton_Left)) return;

            HideMouse();
            blockingAppInputs = false;
        }
        else if (triggerState == TriggerStopped && !blockingAppInputs)
        {
            ShowMouse();
            blockingAppInputs = true;
        }
        else return;

        EventHandler::SetBlockAppInputs(blockingAppInputs);
        EventHandler::ConsumeEvent();
    }

    template <class EntityType>
    void EditorLayer::EntityWidget(EntityType& e, entt::basic_registry<EntityType>& reg, const bool dropTarget)
    {
        ImGui::PushID(ToInt(entt::to_integral(e)));

        if (reg.valid(e)) ImGui::Text("ID: %d", entt::to_integral(e));
        else ImGui::Text("Invalid Entity");

        if (reg.valid(e) && ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
        {
            ImGui::SetDragDropPayload(MM_IEEE_IMGUI_PAYLOAD_TYPE_ENTITY, &e, sizeof(e));
            ImGui::Text("ID: %d", entt::to_integral(e));
            ImGui::EndDragDropSource();
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

    std::shared_ptr<Scene> EditorLayer::CreateScene(const char* sceneName) const
    {
        return _sceneSerializer->CreateEmptySceneFile(sceneName);
    }

    void EditorLayer::SaveScene(const InputValue inputValue)
    {
        if (inputValue.GetTriggerState() != TriggerStarted) return;

        ANE_ELOG("Scene has been saved");
        _sceneSerializer->Serialize(_activeScene);
    }

    void EditorLayer::SaveScene()
    {
        ANE_ELOG("Scene has been saved");
        _sceneSerializer->Serialize(_activeScene);
    }
}
