#pragma once

#include "ANE/Core/Core.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

//solves warnings
#pragma warning(push, 0)
#include <spdlog/spdlog.h>

#include "LoggingTypes.h"
#include "LogSink.h"
#pragma warning(pop)

namespace Engine
{
    using log_msg = spdlog::details::log_msg;

    class ANE_API Logging
    {
    public:
        /**
        * Initializes formatting and fixed loggers
        */
        static void Init(const std::string& appName = "App");

        /**
        * Returns the fixed engine logger
        */
        static spdlog::logger& GetEngineLogger();
        /**
        * Returns the fixed app logger
        */
        static spdlog::logger& GetAppLogger();
        /**
        * Returns a logger from the registry. If none of type exist its created
        */
        static spdlog::logger& GetLogger(std::string const& name);

        /**
        * Returns a list of unformatted logging messages
        */
        static const std::list<LogMessage>& GetMessages();
        /**
        * Clears the list of logging messages
        */
        static void ClearMessages();

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

#define ANE_INTERNAL_LOGGER_CALL(logger, level, ...) \
(logger).log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, level, __VA_ARGS__)

// Engine log macros
#define ANE_ELOG(...)               ANE_INTERNAL_LOGGER_CALL(ENGINE_LOGGER(), spdlog::level::trace, __VA_ARGS__)
#define ANE_ELOG_DEBUG(...)         ANE_INTERNAL_LOGGER_CALL(ENGINE_LOGGER(), spdlog::level::debug, __VA_ARGS__)
#define ANE_ELOG_INFO(...)          ANE_INTERNAL_LOGGER_CALL(ENGINE_LOGGER(), spdlog::level::info, __VA_ARGS__)
#define ANE_ELOG_WARN(...)          ANE_INTERNAL_LOGGER_CALL(ENGINE_LOGGER(), spdlog::level::warn, __VA_ARGS__)
#define ANE_ELOG_ERROR(...)         ANE_INTERNAL_LOGGER_CALL(ENGINE_LOGGER(), spdlog::level::err, __VA_ARGS__)
#define ANE_ELOG_CRITICAL(...)      ANE_INTERNAL_LOGGER_CALL(ENGINE_LOGGER(), spdlog::level::critical, __VA_ARGS__)

// App log macros
#define ANE_LOG(...)                ANE_INTERNAL_LOGGER_CALL(APP_LOGGER(), spdlog::level::trace, __VA_ARGS__)
#define ANE_LOG_DEBUG(...)          ANE_INTERNAL_LOGGER_CALL(APP_LOGGER(), spdlog::level::debug, __VA_ARGS__)
#define ANE_LOG_INFO(...)           ANE_INTERNAL_LOGGER_CALL(APP_LOGGER(), spdlog::level::info, __VA_ARGS__)
#define ANE_LOG_WARN(...)           ANE_INTERNAL_LOGGER_CALL(APP_LOGGER(), spdlog::level::warn, __VA_ARGS__)
#define ANE_LOG_ERROR(...)          ANE_INTERNAL_LOGGER_CALL(APP_LOGGER(), spdlog::level::err, __VA_ARGS__)
#define ANE_LOG_CRITICAL(...)       ANE_INTERNAL_LOGGER_CALL(APP_LOGGER(), spdlog::level::critical, __VA_ARGS__)

#else
#define ANE_ELOG(...)
#define ANE_ELOG_DEBUG(...)
#define ANE_ELOG_INFO(...)
#define ANE_ELOG_WARN(...)
#define ANE_ELOG_ERROR(...)
#define ANE_ELOG_CRITICAL(...)

#define ANE_LOG(...)
#define ANE_LOG_DEBUG(...)
#define ANE_LOG_INFO(...)
#define ANE_LOG_WARN(...)
#define ANE_LOG_ERROR(...)
#define ANE_LOG_CRITICAL(...)
#endif
