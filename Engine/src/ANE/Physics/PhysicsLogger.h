#pragma once
#include <reactphysics3d/utils/Logger.h>

namespace Engine
{
    class PhysicsLogger : public reactphysics3d::Logger
    {
    public:
        void log(Level level, const std::string& physicsWorldName, Category category, const std::string& message, const char* filename, int lineNumber) override;
    };
}
