#pragma once
#include "ANE/Core/Core.h"

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

//solves warnings
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include "LoggingTypes.h"
#include "LogMessage.h"
#pragma warning(pop)

//todo: write a custom sink for logfilewriter so that you can get a full stacktrace

namespace Engine
{
    class LogFileWriter;
    using log_msg = spdlog::details::log_msg;

    class Logging
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
        * Returns a list of the names of registered loggers
        */
        static const std::vector<std::string>& GetRegisteredLoggerNames();

        /**
        * Returns a name of registered logger by index
        */
        static const std::string& GetRegisteredLoggerName(LoggerIndex loggerId);

        /**
        * Returns a name of registered loggers index by name
        */
        static LoggerIndex GetRegisteredLoggerIndex(std::string const& name);

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

    public:
        static spdlog::pattern_formatter LoggerNameFormatter;
        static spdlog::pattern_formatter SourceFormatter;
        static spdlog::pattern_formatter TimeFormatter;

    private:
        inline static std::list<LogMessage> _logMessages{100};
        inline static std::vector<std::string> _registeredLoggers {};
        static LogFileWriter _writer;
    };
}

#ifndef ANE_DIST

#define ENGINE_LOGGER() Engine::Logging::GetEngineLogger()
#define APP_LOGGER() Engine::Logging::GetAppLogger()

#define ANE_INTERNAL_LOGGER_CALL(logger, level, ...) \
(logger).log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, level, __VA_ARGS__)

#define ANE_INTERNAL_LOGGER_CALL_SOURCE(logger, level, fileName, lineNumber, ...) \
(logger).log(spdlog::source_loc{fileName, lineNumber, SPDLOG_FUNCTION}, level, __VA_ARGS__)

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
#define ANE_ELOG(...) ((void)0)
#define ANE_ELOG_DEBUG(...) ((void)0)
#define ANE_ELOG_INFO(...) ((void)0)
#define ANE_ELOG_WARN(...) ((void)0)
#define ANE_ELOG_ERROR(...) ((void)0)
#define ANE_ELOG_CRITICAL(...) ((void)0)

#define ANE_LOG(...) ((void)0)
#define ANE_LOG_DEBUG(...) ((void)0)
#define ANE_LOG_INFO(...) ((void)0)
#define ANE_LOG_WARN(...) ((void)0)
#define ANE_LOG_ERROR(...) ((void)0)
#define ANE_LOG_CRITICAL(...) ((void)0)
#endif
