#pragma once

#include "Core.h"

namespace Engine
{
    class NP_API Application
    {
    public:
        Application();
        virtual ~Application();
        
        void Run();
    };
}
