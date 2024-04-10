#pragma once

#include "Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/gtx/string_cast.hpp"

//solves warnings
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)

namespace Engine
{
    class ANE_API Logging
    {
    public:
        static void InitEngine();
        static void InitApp(const std::string& appName = "APP");

        static std::shared_ptr<spdlog::logger>& GetEngineLogger()
        {
            if(_engineLogger == nullptr)
            {
                spdlog::log(spdlog::level::level_enum::err, "Engine logger not yet initialized");
            }
            return _engineLogger;
        }
        static std::shared_ptr<spdlog::logger>& GetAppLogger()
        {
            if(_appLogger == nullptr)
            {
                GetEngineLogger()->error("App logger not yet initialized");
            }
            return _appLogger;
        }
    private:
        static std::shared_ptr<spdlog::logger> _engineLogger;
        static std::shared_ptr<spdlog::logger> _appLogger;
    };
}

#ifndef ANE_DIST
// Engine log macros
#define ANE_ENGINE_LOG_TRACE(...)    ::Engine::Logging::GetEngineLogger()->trace(__VA_ARGS__)
#define ANE_ENGINE_LOG_INFO(...)     ::Engine::Logging::GetEngineLogger()->info(__VA_ARGS__)
#define ANE_ENGINE_LOG_WARN(...)     ::Engine::Logging::GetEngineLogger()->warn(__VA_ARGS__)
#define ANE_ENGINE_LOG_ERROR(...)    ::Engine::Logging::GetEngineLogger()->error(__VA_ARGS__)
#define ANE_ENGINE_LOG_CRITICAL(...) ::Engine::Logging::GetEngineLogger()->critical(__VA_ARGS__)

// App log macros
#define ANE_LOG_TRACE(...)         ::Engine::Logging::GetAppLogger()->trace(__VA_ARGS__)
#define ANE_LOG_INFO(...)          ::Engine::Logging::GetAppLogger()->info(__VA_ARGS__)
#define ANE_LOG_WARN(...)          ::Engine::Logging::GetAppLogger()->warn(__VA_ARGS__)
#define ANE_LOG_ERROR(...)         ::Engine::Logging::GetAppLogger()->error(__VA_ARGS__)
#define ANE_LOG_CRITICAL(...)      ::Engine::Logging::GetAppLogger()->critical(__VA_ARGS__)
#else
#define ANE_ENGINE_LOG_TRACE(...)
#define ANE_ENGINE_LOG_INFO(...)
#define ANE_ENGINE_LOG_WARN(...)
#define ANE_ENGINE_LOG_ERROR(...)
#define ANE_ENGINE_LOG_CRITICAL(...)

#define ANE_LOG_TRACE(...)
#define ANE_LOG_INFO(...)
#define ANE_LOG_WARN(...)
#define ANE_LOG_ERROR(...)
#define ANE_LOG_CRITICAL(...)
#endif
