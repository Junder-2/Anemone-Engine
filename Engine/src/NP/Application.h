#pragma once

#include "Core.h"
#include "LayerStack.h"
#include "Window.h"
#include "Input/InputManager.h"

namespace Engine
{
    struct NP_API ApplicationSpecification
    {
        std::string Name = "NP Application";
        std::string LogName = "NP APP";
    };

    class NP_API Application
    {
    public:
        Application(const ApplicationSpecification& specification);
        virtual ~Application();

        void Run();
        void Shutdown();
        void OnResize(int width, int height);
        void OnKeyTest(InputValue inputValue);
        void OnMouseKeyTest(MouseButtonValue inputValue);
        void OnMouseMoveTest(MouseMoveValue inputValue);

        Window& GetWindowContext() const { return *_windowContext; }
        InputManager& GetInputManager() const { return *_inputManager.get(); }

        static Application& Get() { return *_appInstance; }

        const ApplicationSpecification& GetAppSpecification() const { return _appSpec; }

    private:
        ApplicationSpecification _appSpec;
        std::unique_ptr<Window> _windowContext;
        std::unique_ptr<InputManager> _inputManager;
        bool _isRunning = true;

        static Application* _appInstance;

        Uint64 _lastTimeStamp;
        LayerStack _layerStack;
    };

    Application* CreateApplication();
}
