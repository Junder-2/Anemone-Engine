#include <cstdio>
#include <NP.h>

class Sandbox : public Engine::Application
{
public:
    Sandbox() { }
    ~Sandbox() { }
};

int main(int arg, char* argv[])
{
    Engine::Logging::Init();

    NP_ENGINE_LOG_INFO("Engine Start");
    NP_LOG_INFO("App Start");
    
    Sandbox* app = new Sandbox();
    app->Run();
    delete app;
    
    return 1;
}
