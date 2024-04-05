#pragma once

#include "Core.h"
#include "Layers/LayerStack.h"
#include "Window.h"
#include "ANE/Events/Event.h"

#include "ANE/Input/InputManager.h"

namespace Engine
{
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

        void Run();
        void Shutdown();

        void OnEvent(Event& e);
        void OnWindowResize(WindowResizeEvent& e);
        void OnWindowMove(WindowMovedEvent& e);
        void OnWindowStateChange(WindowStateChangeEvent& e);
        void OnWindowFocusChange(WindowFocusChangeEvent& e);

        //Test functions
        void OnResizeTest(int width, int height);
        void OnKeyTest(KeyTriggerEvent& keyTriggerEvent);
        void OnAxisTest(InputValue inputValue);
        void OnMouseKeyTest(MouseButtonEvent& mouseButtonEvent);
        void OnMouseScrollTest(MouseScrollEvent& mouseScrollEvent);
        void OnMouseMoveTest(MouseMovementEvent& mouseMovementEvent);

        Window& GetWindow() const { return *_window; }
        InputManager& GetInputManager() const { return *_inputManager.get(); }

        static Application& Get() { return *_appInstance; }

        const ApplicationSpecification& GetAppSpecification() const { return _appSpec; }

    private:
        ApplicationSpecification _appSpec;
        std::unique_ptr<Window> _window;
        std::unique_ptr<InputManager> _inputManager;
        bool _isRunning = true;

        static Application* _appInstance;

        Uint64 _lastTimeStamp;
        LayerStack _layerStack;
    };

    Application* CreateApplication();
}
