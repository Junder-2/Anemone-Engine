#include <cstdio>
#include <..\..\Engine\src\ANE.h>

class Sandbox : public Engine::Application
{
public:
    Sandbox(const Engine::ApplicationSpecification& specification) : Application(specification) { }
    ~Sandbox() { }
};

std::string Engine::GetApplicationName()
{
    return "Sandbox";
}

Engine::Application* Engine::CreateApplication()
{
    ApplicationSpecification spec;
    spec.Name = GetApplicationName();
    spec.LogName = "SANDBOX";

    return new Sandbox(spec);
}
