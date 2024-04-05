#include "anepch.h"
#include "Application.h"

#include <bitset>

#include <SDL_timer.h>

#include "ANE/Events/EventHandler.h"
#include "Layers/Layer.h"
#include "Entity/Entity.h"
#include "Layers/EditorLayer.h"
#include "ANE/Renderer/Renderer.h"
#include "Scene/Components/RenderComponent.h"

namespace Engine
{
    Application* Application::_appInstance = nullptr;

    Application::Application(const ApplicationSpecification& specification) : _appSpec(specification)
    {
        _appInstance = this;

        _window = Window::Create(WindowProperties(_appSpec.Name));
        _window->EventDelegate = MakeDelegate(this, &Application::OnEvent);

        _inputManager = InputManager::Create();
        _inputManager->EventDelegate = MakeDelegate(this, &Application::OnEvent);


        Renderer::Init(_window->GetWindowContext());

        //todo: make into template method
        EditorLayer* editorLayer = new EditorLayer("EditorLayer");
        editorLayer->AddScene<Scene>("Game"); //
        //editorLayer->SetActiveScene("Game");
        Entity ent = editorLayer->GetActiveScene()->Create("Square Entity");
        ent.AddComponent<RenderComponent>();
        if (RenderComponent comp; ent.TryGetComponent<RenderComponent>(comp))
        {
            TagComponent tag;
            ent.TryGetComponent(tag);
            ANE_ENGINE_LOG_WARN("We have a renderComponent with tag: {0} on entity: {1}", comp.ToString(), tag.Tag);
        }
        //editorLayer->AddScene<Scene>("Main Menu");
        //editorLayer->AddScene<Scene>("Credits");
        _layerStack.PushLayer(editorLayer);

        //editorLayer->SetActiveScene("roe");
    }

    Application::~Application() = default;

    void Application::Run()
    {
        while (_isRunning)
        {
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
            //Renderer::Present()
            //Split this so inputs get processed before everything else
        }

        // TODO: Figure out where this function should go.
        Renderer::Shutdown();
    }

    void Application::OnEvent(Event& e)
    {
        EventHandler::PushEvent(&e);

        if(e.HasCategory(WindowEvent))
        {
            switch (e.GetEventType())
            {
                case EventType::WindowClose:
                    Shutdown();
                break;
                case EventType::WindowResize:
                    OnWindowResize(dynamic_cast<WindowResizeEvent&>(e));
                break;
                case EventType::WindowMoved:
                    OnWindowMove(dynamic_cast<WindowMovedEvent&>(e));
                break;
                case EventType::WindowFocusChange:
                    OnWindowFocusChange(dynamic_cast<WindowFocusChangeEvent&>(e));
                break;
                case EventType::WindowStateChange:
                    OnWindowStateChange(dynamic_cast<WindowStateChangeEvent&>(e));
                break;
            }
        }

        //Input debugging
        // if(e.HasCategory(InputEvent))
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
    }

    void Application::OnWindowResize(WindowResizeEvent& e)
    {
        ANE_ENGINE_LOG_INFO("new size ({0}, {1})", e.GetWidth(), e.GetHeight());
    }

    void Application::OnWindowMove(WindowMovedEvent& e)
    {
        ANE_ENGINE_LOG_INFO("new pos ({0}, {1}) : ({2}, {3})", e.GetX(), e.GetY(), e.GetXDelta(), e.GetYDelta());
    }

    void Application::OnWindowStateChange(WindowStateChangeEvent& e)
    {
        ANE_ENGINE_LOG_INFO("window state change {0}", (int)e.GetState());
    }

    void Application::OnWindowFocusChange(WindowFocusChangeEvent& e)
    {
        ANE_ENGINE_LOG_INFO("window focus change {0}", e.IsFocused());
    }

    void Application::OnKeyTest(KeyTriggerEvent& keyTriggerEvent)
    {
        const InputValue inputValue = keyTriggerEvent;
        ANE_ENGINE_LOG_INFO("pressed {0}: {1}", inputValue.GetBindingId(), inputValue.GetIntValue());
    }

    void Application::OnAxisTest(InputValue inputValue)
    {
        ANE_ENGINE_LOG_INFO("pressed {0}: {1}", inputValue.GetBindingId(), inputValue.GetAxis());
    }

    void Application::OnMouseKeyTest(MouseButtonEvent& mouseButtonEvent)
    {
        const MouseButtonValues inputValue = mouseButtonEvent;
        ANE_ENGINE_LOG_INFO("pressed mouse key {0}, with state {1}, is doubleclick {2}", inputValue.GetCurrentButtonIndex(), (int)inputValue.GetTriggerState(), inputValue.GetIsDoubleClick());
        ANE_ENGINE_LOG_INFO("raw mouse button state {0}", std::bitset<16>(inputValue.GetRawButtonStates()).to_string());
    }

    void Application::OnMouseScrollTest(MouseScrollEvent& mouseScrollEvent)
    {
        ANE_ENGINE_LOG_INFO("scrolled mouse ({0}, {1})", mouseScrollEvent.GetXDelta(), mouseScrollEvent.GetYDelta());
    }

    void Application::OnMouseMoveTest(MouseMovementEvent& mouseMovementEvent)
    {
        const MouseMoveValue inputValue = mouseMovementEvent;
        // spdlog formatting not working use explicit glm::to_string
        ANE_ENGINE_LOG_INFO("moved mouse pos:({0}), delta:({1})", glm::to_string(inputValue.GetMousePos()), glm::to_string(inputValue.GetMouseDelta()));
    }
}
