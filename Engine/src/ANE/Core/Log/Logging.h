#pragma once

#include "ANE/Core/Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_LEFT_HANDED
#include "glm/gtx/string_cast.hpp"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

//solves warnings
#pragma warning(push, 0)
#include <entt.hpp>
#include <spdlog/spdlog.h>

#include "LogSink.h"
#pragma warning(pop)

namespace Engine
{
    using log_msg = spdlog::details::log_msg;

    //log_msg
    struct LogMessage
    {
        std::string Time;
        std::string Source;
        std::string LoggerName;
        spdlog::level::level_enum Level;
        std::string Message;
    };

    class ANE_API Logging
    {
    public:
        static void Init(const std::string& appName = "App");

        static spdlog::logger& GetEngineLogger();
        static spdlog::logger& GetAppLogger();
        static spdlog::logger& GetLogger(std::string const& name);

        static const std::list<LogMessage>& GetMessages();

    private:
        static std::shared_ptr<spdlog::logger> CreateLogger(const std::string& loggerName);
        static void OnSink(const log_msg& logMsg);

    private:
        inline static std::list<LogMessage> _logMessages {100};
    };


}

#ifndef ANE_DIST

#define ENGINE_LOGGER() Engine::Logging::GetEngineLogger()
#define APP_LOGGER() Engine::Logging::GetAppLogger()

#define ANE_INTERNAL_ELOG(LEVEL, ...) ASLOG_TO_LOGGER(ENGINE_LOGGER(), LEVEL, __VA_ARGS__)
#define ANE_INTERNAL_LOG(LEVEL, ...) ASLOG_TO_LOGGER(APP_LOGGER(), LEVEL, __VA_ARGS__)

#define ANE_INTERNAL_LOGGER_CALL(logger, level, ...) \
(logger).log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, level, __VA_ARGS__)

// Engine log macros
#define ANE_ELOG(...)               ANE_INTERNAL_LOGGER_CALL(ENGINE_LOGGER(), spdlog::level::trace, __VA_ARGS__)
#define ANE_ELOG_INFO(...)          ANE_INTERNAL_LOGGER_CALL(ENGINE_LOGGER(), spdlog::level::info, __VA_ARGS__)
#define ANE_ELOG_WARN(...)          ANE_INTERNAL_LOGGER_CALL(ENGINE_LOGGER(), spdlog::level::warn, __VA_ARGS__)
#define ANE_ELOG_ERROR(...)         ANE_INTERNAL_LOGGER_CALL(ENGINE_LOGGER(), spdlog::level::err, __VA_ARGS__)
#define ANE_ELOG_CRITICAL(...)      ANE_INTERNAL_LOGGER_CALL(ENGINE_LOGGER(), spdlog::level::critical, __VA_ARGS__)

// App log macros
#define ANE_LOG(...)                ANE_INTERNAL_LOGGER_CALL(APP_LOGGER(), spdlog::level::trace, __VA_ARGS__)
#define ANE_LOG_INFO(...)           ANE_INTERNAL_LOGGER_CALL(APP_LOGGER(), spdlog::level::info, __VA_ARGS__)
#define ANE_LOG_WARN(...)           ANE_INTERNAL_LOGGER_CALL(APP_LOGGER(), spdlog::level::warn, __VA_ARGS__)
#define ANE_LOG_ERROR(...)          ANE_INTERNAL_LOGGER_CALL(APP_LOGGER(), spdlog::level::err, __VA_ARGS__)
#define ANE_LOG_CRITICAL(...)       ANE_INTERNAL_LOGGER_CALL(APP_LOGGER(), spdlog::level::critical, __VA_ARGS__)

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
