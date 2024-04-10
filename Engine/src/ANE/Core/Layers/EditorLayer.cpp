#include "anepch.h"
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

namespace Engine
{
    EditorLayer::EditorLayer(const std::string& name) : Layer(name)
    {
    }

    EditorLayer::~EditorLayer() = default;

    void EditorLayer::OnAttach()
    {
        // You would have a "Read from config files to find correct panel layout" method here

        // Then you would call load methods to load the most recent project

        //Then you would load the scene from the file path listed from that project

        CreateTestScene();
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
        ImGui::Begin("Hello");
        ImGui::Button("Button");
        ImGui::End();

        static bool showSimpleOverlay = true;
        if (showSimpleOverlay) ShowInputDebugOverlay(&showSimpleOverlay);

        ImGui::ShowDemoWindow();
    }

    void EditorLayer::OnUpdate(float deltaTime)
    {
        if (_activeScene) _activeScene->OnUpdate(deltaTime);
    }

    void EditorLayer::CreateTestScene()
    {
        //Add scene to layer
        AddScene<Scene>("Game");

        //Create a Entity
        Entity ent = GetActiveScene()->Create("Square Entity");

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
            const Vector2 mousePos = inputManager.GetMousePos();
            ImGui::Text("Mouse Pos: (%.3f,%.3f)", mousePos.X, mousePos.Y);
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
}
