#include "anepch.h"
#include "PhysicsLogger.h"

namespace Engine
{
    void PhysicsLogger::log(Level level, const std::string& physicsWorldName, Category category, const std::string& message, const char* filename, int lineNumber)
    {
        std::string cleanedString = message;
        std::replace(cleanedString.begin(), cleanedString.end(), '\n', ' ');

        switch (level)
        {
            case Level::Information: ANE_PHYS_LOG_INFO("({0}, {1}): {2}", physicsWorldName, getCategoryName(category), cleanedString); break;
            case Level::Warning: ANE_PHYS_LOG_WARN("({0}, {1}): {2}", physicsWorldName, getCategoryName(category), cleanedString); break;
            case Level::Error: ANE_PHYS_LOG_ERROR("({0}, {1}): {2}", physicsWorldName, getCategoryName(category), cleanedString); break;
        }
    }

    spdlog::logger& PhysicsLogger::GetPhysicsLogger()
    {
        return Logging::GetLogger("Physics");
    }
}
