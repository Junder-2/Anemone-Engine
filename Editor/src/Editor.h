#pragma once

#include <..\..\Engine\src\ANE.h>

class Editor : public Engine::Application
{
public:
    Editor(const Engine::ApplicationSpecification& specification) : Application(specification) { }
    ~Editor() { }
};
