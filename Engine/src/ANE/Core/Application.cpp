#include "anepch.h"
#include "Application.h"

#include <bitset>

#include <SDL_timer.h>

#include "Window.h"
#include "ANE/Events/Event.h"
#include "ANE/Events/EventHandler.h"
#include "ANE/Subsystem/SubSystemCollection.h"
#include "ANE/Input/InputHandler.h"
#include "ANE/Utilities/InputUtilities.h"
#include "Layers/Layer.h"
#include "Entity/Entity.h" // this holds something that Rendercomponent needs?
#include "ANE/Utilities/API.h"
#include "ANE/Renderer/Renderer.h"
#include "Layers/ImGuiLayer.h"
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

        _imGuiLayer = ImGuiLayer::Create("ImGuiLayer");
        PushLayer(_imGuiLayer);

        API::WINDOW_SIZE = Vector2((float)_window->GetWindowWidth(), (float)_window->GetWindowHeight());
    }

    void Application::Run()
    {
        _lastTimeStamp = SDL_GetPerformanceCounter();

        while (_isRunning)
        {
            ANE_DEEP_PROFILE_SCOPE("Application::Run");

            const Uint64 timeStamp = SDL_GetPerformanceCounter();
            const Uint64 timeStep = timeStamp - _lastTimeStamp;
            _lastTimeStamp = timeStamp;

            float deltaTime = timeStep / static_cast<float>(SDL_GetPerformanceFrequency());

            _window->OnUpdate(deltaTime);

            //Renderer
            for (Layer* layer : _layerStack)
            {
                layer->OnUpdate(deltaTime);
            }

            Renderer::BeginUIDataBuffer();
            for (Layer* layer : _layerStack)
            {
                layer->OnUIRender();
            }
            Renderer::EndUIDataBuffer();

            Renderer::Render(_window->GetWindowProperties());
        }

        Renderer::Shutdown();
    }

    void Application::OnEvent(Event& e)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        EventHandler::PushEvent(&e);

        if(e.HasCategory(EventCategoryWindow))
        {
            switch (e.GetEventType())
            {
                case EventType::WindowClose:
                    Shutdown();
                break;
                case EventType::WindowResize:
                    OnWindowResize(reinterpret_cast<WindowResizeEvent&>(e));
                break;
                case EventType::WindowMoved:
                    OnWindowMove(reinterpret_cast<WindowMovedEvent&>(e));
                break;
                // case EventType::WindowFocusChange:
                //     OnWindowFocusChange(reinterpret_cast<WindowFocusChangeEvent&>(e));
                // break;
                // case EventType::WindowStateChange:
                //     OnWindowStateChange(reinterpret_cast<WindowStateChangeEvent&>(e));
                // break;
            }
        }

        Renderer::OnEvent(e);

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

    void Application::OnWindowResize(const WindowResizeEvent& e)
    {
        if(e.IsMainWindow())
        {
            API::WINDOW_SIZE = Vector2((float)e.GetWidth(), (float)e.GetHeight());
        }
        else if(e.IsActiveViewport())
        {
            API::VIEWPORT_SIZE = Vector2((float)e.GetWidth(), (float)e.GetHeight());
        }
        //ANE_ELOG_DEBUG("new size {0} ({1}, {2})", e.GetWindowIndex(), e.GetWidth(), e.GetHeight());
    }

    void Application::OnWindowMove(const WindowMovedEvent& e)
    {
        if(e.IsMainWindow())
        {
            API::WINDOW_POS = Vector2((float)e.GetX(), (float)e.GetY());
        }
        else if(e.IsActiveViewport())
        {
            API::VIEWPORT_POS = Vector2((float)e.GetX(), (float)e.GetY());
        }
        //ANE_ELOG_DEBUG("new pos {0} ({1}, {2})",e.GetWindowIndex(), e.GetX(), e.GetY());
    }

    void Application::OnWindowStateChange(WindowStateChangeEvent& e)
    {
        ANE_ELOG_DEBUG("window state change {0}", (int)e.GetState());
    }

    void Application::OnWindowFocusChange(WindowFocusChangeEvent& e)
    {
        ANE_ELOG_DEBUG("window focus change {0}", e.IsFocused());
    }
}
