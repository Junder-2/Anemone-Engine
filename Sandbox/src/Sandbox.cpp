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
    Sandbox* app = new Sandbox();
    app->Run();
    delete app;
    
    return 1;
}
