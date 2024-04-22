#include "anepch.h"
#include "Application.h"

#include <bitset>

#include <SDL_timer.h>

#include "ANE/Events/EventHandler.h"
#include "ANE/Subsystem/SubSystemCollection.h"
#include "ANE/Input/Input.h"
#include "ANE/Utilities/InputUtilities.h"
#include "Layers/Layer.h"
#include "Entity/Entity.h"
#include "Entity/ExampleScripts/CameraController.h"
#include "Layers/EditorLayer.h"
#include "ANE/Renderer/Renderer.h"
#include "Scene/Components/CameraComponent.h"
#include "Scene/Components/NativeScriptComponent.h"
#include "Scene/Components/RenderComponent.h"

namespace Engine
{
    Application* Application::_appInstance = nullptr;

    Application::Application(const ApplicationSpecification& specification) : _appSpec(specification)
    {
        _appInstance = this;

        Init();
    }

    Application::~Application() = default;

    void Application::Init()
    {
        ANE_PROFILE_FUNCTION();
        _window = Window::Create(WindowProperties(_appSpec.Name));
        _window->BindOnEvent(MakeDelegate(this, &Application::OnEvent));

        Renderer::Init(_window->GetWindowContext());

        _inputHandler = InputHandler::Create();
        _inputHandler->BindOnEvent(MakeDelegate(this, &Application::OnEvent));

        _subsystemCollection = SubSystemCollection::Create();

        _ImGuiLayer = ImGuiLayer::Create("ImGuiLayer");
        PushLayer(_ImGuiLayer);
    }

    void Application::Run()
    {
        while (_isRunning)
        {
            ANE_DEEP_PROFILE_SCOPE("Application::Run");

            const Uint64 timeStamp = SDL_GetPerformanceCounter();
            const Uint64 timeStep = timeStamp - _lastTimeStamp;
            _lastTimeStamp = timeStamp;

            float deltaTime = timeStep / static_cast<float>(SDL_GetPerformanceFrequency());

            _window->OnUpdate(deltaTime);

            //Renderer
            for (Layer* layer : _layerStack) // raw pointers
            {
                layer->OnUpdate(deltaTime);
            }
            //todo frame yap

            Renderer::BeginUiDataBuffer();
            for (Layer* layer : _layerStack) // raw pointers
            {
                layer->OnUIRender();
            }
            Renderer::EndUIDataBuffer();

            Renderer::Render(_window->GetProperties());
        }

        // TODO: Figure out where this function should go.
        Renderer::Shutdown();
    }

    void Application::OnEvent(Event& e)
    {
        EventHandler::PushEvent(&e);

        if(e.HasCategory(EventCategoryWindow))
        {
            switch (e.GetEventType())
            {
                case EventType::WindowClose:
                    Shutdown();
                break;
                case EventType::WindowResize:
                    OnWindowResize(dynamic_cast<WindowResizeEvent&>(e));
                break;
                // case EventType::WindowMoved:
                //     OnWindowMove(dynamic_cast<WindowMovedEvent&>(e));
                // break;
                // case EventType::WindowFocusChange:
                //     OnWindowFocusChange(dynamic_cast<WindowFocusChangeEvent&>(e));
                // break;
                // case EventType::WindowStateChange:
                //     OnWindowStateChange(dynamic_cast<WindowStateChangeEvent&>(e));
                // break;
            }
        }

        //Input debugging
        // if(e.HasCategory(EventCategoryInput))
        // {
        //     switch (e.GetEventType())
        //     {
        //         case EventType::KeyboardInput:
        //             OnKeyTest(dynamic_cast<KeyTriggerEvent&>(e));
        //         break;
        //         case EventType::MouseButton:
        //             OnMouseKeyTest(dynamic_cast<MouseButtonEvent&>(e));
        //         break;
        //         case EventType::MouseScrolled:
        //             OnMouseScrollTest(dynamic_cast<MouseScrollEvent&>(e));
        //         break;
        //         case EventType::MouseMovement:
        //             OnMouseMoveTest(dynamic_cast<MouseMovementEvent&>(e));
        //         break;
        //     }
        // }

        //todo: should send to renderer

        for (Layer* layer : _layerStack) // raw pointers
        {
            if(e.IsConsumed()) break;
            layer->OnEvent(e);
        }

        EventHandler::ClearEvent();
    }

    void Application::Shutdown()
    {
        _isRunning = false;
        EventHandler::ConsumeEvent();
    }

    void Application::PushLayer(Layer* layer)
    {
        _layerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::OnWindowResize(WindowResizeEvent& e)
    {
        ANE_ELOG_DEBUG("new size ({0}, {1})", e.GetWidth(), e.GetHeight());
    }

    void Application::OnWindowMove(WindowMovedEvent& e)
    {
        ANE_ELOG_DEBUG("new pos ({0}, {1}) : ({2}, {3})", e.GetX(), e.GetY(), e.GetXDelta(), e.GetYDelta());
    }

    void Application::OnWindowStateChange(WindowStateChangeEvent& e)
    {
        ANE_ELOG_DEBUG("window state change {0}", (int)e.GetState());
    }

    void Application::OnWindowFocusChange(WindowFocusChangeEvent& e)
    {
        ANE_ELOG_DEBUG("window focus change {0}", e.IsFocused());
    }

    void Application::OnKeyTest(KeyboardKeyEvent& keyTriggerEvent)
    {
        const InputValue inputValue = keyTriggerEvent.GetInputValue();
        ANE_ELOG_DEBUG("pressed {0}: {1}", inputValue.GetBindingId(), InputUtilities::ToString(inputValue.GetTriggerState()));
    }

    void Application::OnMouseKeyTest(MouseButtonEvent& mouseButtonEvent)
    {
        const MouseButtonValues inputValue = mouseButtonEvent.GetInputValue();
        ANE_ELOG_DEBUG("pressed mouse key {0}, with {1}, is doubleclick {2}", InputUtilities::ToString((MouseButton)inputValue.GetCurrentButtonIndex()),
            InputUtilities::ToString(inputValue.GetTriggerState()), inputValue.GetIsDoubleClick());
        ANE_ELOG_DEBUG("raw mouse button state {0}", std::bitset<16>(inputValue.GetRawButtonStates()).to_string());
    }

    void Application::OnMouseScrollTest(MouseScrollEvent& mouseScrollEvent)
    {
        ANE_ELOG_DEBUG("scrolled mouse ({0}, {1})", mouseScrollEvent.GetXDelta(), mouseScrollEvent.GetYDelta());
    }

    void Application::OnMouseMoveTest(MouseMovementEvent& mouseMovementEvent)
    {
        const MouseMoveValue inputValue = mouseMovementEvent.GetInputValue();
        ANE_ELOG_DEBUG("moved mouse pos:({0}), delta:({1})", inputValue.GetMousePos().ToString(), inputValue.GetMouseDelta().ToString());
    }
}
