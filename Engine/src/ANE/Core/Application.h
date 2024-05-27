#pragma once
#include "Layers/LayerStack.h"

namespace Engine
{
    class MouseMovementEvent;
    class MouseScrollEvent;
    class MouseButtonEvent;
    class KeyboardKeyEvent;
    class WindowFocusChangeEvent;
    class WindowStateChangeEvent;
    class WindowMovedEvent;
    class WindowResizeEvent;
    class Event;
    class ImGuiLayer;
    class InputHandler;
    class Window;
    class Layer;
    class SubSystemCollection;

    struct ApplicationSpecification
    {
        std::string Name = "Anemone Application";
    };

    class Application
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
        ImGuiLayer& GetImGuiLayer() const { return *_imGuiLayer; }
        SubSystemCollection& GetSubsystemCollection() const { return *_subsystemCollection; }

        static Application& Get() { return *_appInstance; }

        const ApplicationSpecification& GetAppSpecification() const { return _appSpec; }

    private:
        void OnEvent(Event& e);
        void OnWindowResize(const WindowResizeEvent& e);
        void OnWindowMove(const WindowMovedEvent& e);
        void OnWindowStateChange(WindowStateChangeEvent& e);
        void OnWindowFocusChange(WindowFocusChangeEvent& e);

    private:
        ApplicationSpecification _appSpec;
        std::unique_ptr<Window> _window;
        std::unique_ptr<InputHandler> _inputHandler;
        std::unique_ptr<SubSystemCollection> _subsystemCollection;

        bool _isRunning = true;

        static Application* _appInstance;

        uint64 _lastTimeStamp;
        LayerStack _layerStack;
        ImGuiLayer* _imGuiLayer;
    };

    Application* CreateApplication();
    std::string GetApplicationName();
}
