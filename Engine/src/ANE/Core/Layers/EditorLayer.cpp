#include "anepch.h"
#include "EditorLayer.h"
#include <imgui.h>

#include "ANE/Core/Entity/Entity.h"
#include "LayerStack.h"
#include "ANE/Events/EventHandler.h"

//Temp includes. Can probably change how these components are referenced. Maybe a scene manager hsould be in charge of that
//kind of thing
#include "ANE/Core/Application.h"
#include "ANE/Core/Entity/ExampleScripts/CameraController.h"
#include "ANE/Core/Scene/Components/CameraComponent.h"
#include "ANE/Core/Scene/Components/NativeScriptComponent.h"
#include "ANE/Core/Scene/Components/RenderComponent.h"
#include "ANE/Input/EditorInputSystem.h"

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

        GetEditorInputSystem().BindKeyboardInput(KeyCodeEscape, MakeDelegate(this, &EditorLayer::OnSwitchEditorFocus));
        GetEditorInputSystem().BindMouseButton(MouseButtonLeft, MakeDelegate(this, &EditorLayer::OnSwitchEditorFocus));
        GetEditorInputSystem().BindMouseButton(MouseButtonRight, MakeDelegate(this, &EditorLayer::OnSwitchEditorFocus));
        GetEditorInputSystem().BindMouseButton(MouseButtonMiddle, MakeDelegate(this, &EditorLayer::OnSwitchEditorFocus));
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
        // ImGui::Begin("Hello");
        // ImGui::Button("Button");
        // ImGui::End();

        //entt::dense_map<std::string, bool> _activeMap;

        // These should be moved later


        //ImGui::ShowDemoWindow();
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
        ent.AddComponent<CameraComponent>();
        ent.AddComponent<NativeScriptComponent>().Bind<CameraController>();

        //Get Component from entity
        if (RenderComponent comp; ent.TryGetComponent<RenderComponent>(comp))
        {
            TagComponent tag;
            ent.TryGetComponent(tag);
            ANE_ELOG_WARN("We have a renderComponent with tag: {0} on entity: {1}", comp.ToString(), tag.Tag);
        }
    }

    void EditorLayer::OnSwitchEditorFocus(InputValue inputValue)
    {
        const bool editorHasFocus = EventHandler::IsBlockingAppInputs();

        if(inputValue.GetDeviceType() == InputDeviceKeyboard)
        {
            switch (inputValue.GetBindingId())
            {
                case KeyCodeEscape:
                    if(inputValue.GetTriggerState() != TriggerStarted) return;
                break;
                default: return;
            }
        }
        else if(inputValue.GetDeviceType() == InputDeviceMouse) //Any mouse click should return focus
        {
            if(!editorHasFocus && inputValue.GetTriggerState() == TriggerStarted)
            {
                ShowMouse();
            }

            if(inputValue.GetTriggerState() != TriggerStarted || !editorHasFocus) return;
        }

        EventHandler::SetBlockAppInputs(!editorHasFocus);
        if(!editorHasFocus)
        {
            HideMouse();
        }
       // _showMenuBar = !editorHasFocus;
        EventHandler::ConsumeEvent();
    }

    //Copied from IMGUI example
    // void EditorLayer::ShowEditorMenuBar()
    // {
    //     if (ImGui::BeginMainMenuBar())
    //     {
    //         if (ImGui::BeginMenu("Debug"))
    //         {
    //             if (ImGui::MenuItem("Input Debug", nullptr, _showInputDebugOverlay))
    //             {
    //                 _showInputDebugOverlay = !_showInputDebugOverlay;
    //             }
    //             ImGui::EndMenu();
    //         }
    //         ImGui::EndMainMenuBar();
    //     }
    // }

    //Copied from IMGUI example

}
