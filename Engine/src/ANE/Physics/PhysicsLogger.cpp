#include "anepch.h"
#include "PhysicsLogger.h"

namespace Engine
{
    void PhysicsLogger::log(Level level, const std::string& physicsWorldName, Category category, const std::string& message, const char* filename, int lineNumber)
    {
        switch (level)
        {
            case Level::Information: ANE_ELOG_INFO("Physics World ({0}, {1}): {2}", physicsWorldName, getCategoryName(category), message); break;
            case Level::Error: ANE_ELOG_ERROR("Physics World ({0}, {1}): {2}", physicsWorldName, getCategoryName(category), message); break;
            case Level::Warning: ANE_ELOG_WARN("Physics World ({0}, {1}): {2}", physicsWorldName, getCategoryName(category), message); break;
        }
    }
}
