#pragma once
#include <reactphysics3d/utils/Logger.h>

namespace Engine
{
    class PhysicsLogger final : public reactphysics3d::Logger
    {
    public:
        void log(Level level, const std::string& physicsWorldName, Category category, const std::string& message, const char* filename, int lineNumber) override;

        static spdlog::logger& GetPhysicsLogger();
    };

    #ifndef ANE_DIST

    #define PHYSICS_LOGGER() Engine::PhysicsLogger::GetPhysicsLogger()

    #define ANE_PHYS_LOG_INFO(fileName, lineNumber, ...)            ANE_INTERNAL_LOGGER_CALL_SOURCE(PHYSICS_LOGGER(), spdlog::level::info, fileName, lineNumber, __VA_ARGS__)
    #define ANE_PHYS_LOG_WARN(fileName, lineNumber, ...)            ANE_INTERNAL_LOGGER_CALL_SOURCE(PHYSICS_LOGGER(), spdlog::level::warn, fileName, lineNumber, __VA_ARGS__)
    #define ANE_PHYS_LOG_ERROR(fileName, lineNumber, ...)           ANE_INTERNAL_LOGGER_CALL_SOURCE(PHYSICS_LOGGER(), spdlog::level::err, fileName, lineNumber, __VA_ARGS__)

    #else
    #define ANE_PHYS_LOG_INFO(fileName, lineNumber, ...)
    #define ANE_PHYS_LOG_WARN(fileName, lineNumber, ...)
    #define ANE_PHYS_LOG_ERROR(fileName, lineNumber, ...)
    #endif
}
