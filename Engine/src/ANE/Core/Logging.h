#pragma once

#include "Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include "glm/gtx/string_cast.hpp"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

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
#define ANE_ELOG(...)               SPDLOG_LOGGER_TRACE(::Engine::Logging::GetEngineLogger(), __VA_ARGS__)
#define ANE_ELOG_INFO(...)          SPDLOG_LOGGER_INFO(::Engine::Logging::GetEngineLogger(), __VA_ARGS__)
#define ANE_ELOG_WARN(...)          SPDLOG_LOGGER_WARN(::Engine::Logging::GetEngineLogger(), __VA_ARGS__)
#define ANE_ELOG_ERROR(...)         SPDLOG_LOGGER_ERROR(::Engine::Logging::GetEngineLogger(), __VA_ARGS__)
#define ANE_ELOG_CRITICAL(...)      SPDLOG_LOGGER_CRITICAL(::Engine::Logging::GetEngineLogger(), __VA_ARGS__)

// App log macros
#define ANE_LOG(...)                SPDLOG_LOGGER_TRACE(::Engine::Logging::GetAppLogger(), __VA_ARGS__)
#define ANE_LOG_INFO(...)           SPDLOG_LOGGER_INFO(::Engine::Logging::GetAppLogger(), __VA_ARGS__)
#define ANE_LOG_WARN(...)           SPDLOG_LOGGER_WARN(::Engine::Logging::GetAppLogger(), __VA_ARGS__)
#define ANE_LOG_ERROR(...)          SPDLOG_LOGGER_ERROR(::Engine::Logging::GetAppLogger(), __VA_ARGS__)
#define ANE_LOG_CRITICAL(...)       SPDLOG_LOGGER_CRITICAL(::Engine::Logging::GetAppLogger(), __VA_ARGS__)

#else
#define ANE_ELOG(...)
#define ANE_ELOG_INFO(...)
#define ANE_ELOG_WARN(...)
#define ANE_ELOG_ERROR(...)
#define ANE_ELOG_CRITICAL(...)

#define ANE_LOG(...)
#define ANE_LOG_INFO(...)
#define ANE_LOG_WARN(...)
#define ANE_LOG_ERROR(...)
#define ANE_LOG_CRITICAL(...)
#endif
