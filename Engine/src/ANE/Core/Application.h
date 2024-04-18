#pragma once

#include "Core.h"
#include "Layers/LayerStack.h"
#include "Window.h"
#include "ANE/Events/Event.h"
#include "ANE/Core/Layers/ImGuiLayer.h"
#include "ANE/Input/InputHandler.h"
#include "Log/Sink.h"
#include "Log/LogSinkBase.h"

namespace Engine
{
    class SubSystemCollection;
    struct ANE_API ApplicationSpecification
    {
        std::string Name = "Anemone Application";
        std::string LogName = "ANE APP";
    };

    class ANE_API Application
    {
    public:
        Application(const ApplicationSpecification& specification);
        virtual ~Application();

        void Init();
        void Run();
        void Shutdown();

        void PushLayer(Layer* layer);

        Window& GetWindow() const { return *_window; }
        InputHandler& GetInputHandler() const { return *_inputHandler; }
        ImGuiLayer& GetImGuiLayer() const { return *_ImGuiLayer; }
        SubSystemCollection& GetSubsystemCollection() const { return *_subsystemCollection; }

        static Application& Get() { return *_appInstance; }

        const ApplicationSpecification& GetAppSpecification() const { return _appSpec; }

    private:
        void OnEvent(Event& e);
        void OnWindowResize(WindowResizeEvent& e);
        void OnWindowMove(WindowMovedEvent& e);
        void OnWindowStateChange(WindowStateChangeEvent& e);
        void OnWindowFocusChange(WindowFocusChangeEvent& e);

        //Test functions
        void OnKeyTest(KeyboardKeyEvent& keyTriggerEvent);
        void OnMouseKeyTest(MouseButtonEvent& mouseButtonEvent);
        void OnMouseScrollTest(MouseScrollEvent& mouseScrollEvent);
        void OnMouseMoveTest(MouseMovementEvent& mouseMovementEvent);

    private:
        ApplicationSpecification _appSpec;
        std::unique_ptr<Window> _window;
        std::unique_ptr<InputHandler> _inputHandler;
        std::unique_ptr<SubSystemCollection> _subsystemCollection;

        bool _isRunning = true;

        static Application* _appInstance;

        Uint64 _lastTimeStamp;
        LayerStack _layerStack;
        ImGuiLayer* _ImGuiLayer;
    };

    Application* CreateApplication();
    std::string GetApplicationName();
}
