#pragma once

#include "Core.h"
#include "LayerStack.h"
#include "Window.h"

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

        Window& GetWindowContext() { return *_windowContext; }

        static Application& Get() { return *_appInstance; }

        const ApplicationSpecification& GetAppSpecification() const { return _appSpec; }

    private:
        ApplicationSpecification _appSpec;
        std::unique_ptr<Window> _windowContext;
        bool _isRunning = true;

        static Application* _appInstance;

        Uint64 _lastTimeStamp;
        LayerStack _layerStack;
    };

    Application* CreateApplication();
}
