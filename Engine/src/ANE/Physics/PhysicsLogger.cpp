#include "anepch.h"
#include "PhysicsLogger.h"

namespace Engine
{
    void PhysicsLogger::log(Level level, const std::string& physicsWorldName, Category category, const std::string& message, const char* filename, int lineNumber)
    {
        if(message.find("Set transform") != std::string::npos) // filter out set transform log
            return;
        if(message.find("Set isSleeping") != std::string::npos) // filter out set sleeping log
            return;

        std::string cleanedString = message;
        std::replace(cleanedString.begin(), cleanedString.end(), '\n', ' ');

        switch (level)
        {
            case Level::Information: ANE_PHYS_LOG_INFO(filename, lineNumber, cleanedString); break;
            case Level::Warning: ANE_PHYS_LOG_WARN(filename, lineNumber, cleanedString); break;
            case Level::Error: ANE_PHYS_LOG_ERROR(filename, lineNumber, cleanedString); break;
        }
    }

    spdlog::logger& PhysicsLogger::GetPhysicsLogger()
    {
        return Logging::GetLogger("Physics");
    }
}
