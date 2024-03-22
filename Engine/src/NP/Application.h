#pragma once

#include "Core.h"
#include "LayerStack.h"

namespace Engine
{
    struct ApplicationSpecification
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

        static Application& Get() { return *appInstance; }

        const ApplicationSpecification& GetAppSpecification() const { return appSpec; }

    private:
        ApplicationSpecification appSpec;
        bool isRunning = true;

        static Application* appInstance;

        LayerStack _layerStack;
    };

    Application* CreateApplication();
}
