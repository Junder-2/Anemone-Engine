#pragma once

#include "Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

//solves warnings
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)

namespace Engine
{
    class NP_API Logging
    {
    public:
        static void InitEngine();
        static void InitApp(const std::string& appName = "APP");

        static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return _engineLogger; }
        static std::shared_ptr<spdlog::logger>& GetAppLogger() { return _appLogger; }
    private:
        static std::shared_ptr<spdlog::logger> _engineLogger;
        static std::shared_ptr<spdlog::logger> _appLogger;
    };
}

// Engine log macros
#define NP_ENGINE_LOG_TRACE(...)    ::Engine::Logging::GetEngineLogger()->trace(__VA_ARGS__)
#define NP_ENGINE_LOG_INFO(...)     ::Engine::Logging::GetEngineLogger()->info(__VA_ARGS__)
#define NP_ENGINE_LOG_WARN(...)     ::Engine::Logging::GetEngineLogger()->warn(__VA_ARGS__)
#define NP_ENGINE_LOG_ERROR(...)    ::Engine::Logging::GetEngineLogger()->error(__VA_ARGS__)
#define NP_ENGINE_LOG_CRITICAL(...) ::Engine::Logging::GetEngineLogger()->critical(__VA_ARGS__)

// App log macros
#define NP_LOG_TRACE(...)         ::Engine::Logging::GetAppLogger()->trace(__VA_ARGS__)
#define NP_LOG_INFO(...)          ::Engine::Logging::GetAppLogger()->info(__VA_ARGS__)
#define NP_LOG_WARN(...)          ::Engine::Logging::GetAppLogger()->warn(__VA_ARGS__)
#define NP_LOG_ERROR(...)         ::Engine::Logging::GetAppLogger()->error(__VA_ARGS__)
#define NP_LOG_CRITICAL(...)      ::Engine::Logging::GetAppLogger()->critical(__VA_ARGS__)
