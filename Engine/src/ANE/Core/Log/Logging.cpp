#include "anepch.h"
#include "Logging.h"

#include "LogFileWriter.h"
#include "LogMessage.h"
#include "LogSink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Engine
{
    const std::string DEFAULT_PATTERN = "%^[%n %L %T] [%s:%#] %v%$";
    const std::string ENGINE_LOGGER_NAME = "Engine";
    static std::string _appLoggerName = "app";

    const std::string LOGGER_NAME_PATTERN = "%n";
    const std::string SOURCE_PATTERN = "%s:%#";
    const std::string TIME_PATTERN = "%T";

    spdlog::pattern_formatter Logging::LoggerNameFormatter {LOGGER_NAME_PATTERN};
    spdlog::pattern_formatter Logging::SourceFormatter {SOURCE_PATTERN};
    spdlog::pattern_formatter Logging::TimeFormatter {TIME_PATTERN};

    LogFileWriter Logging::_writer {};

    void Logging::Init(const std::string& appName)
    {
        ANE_PROFILE_FUNCTION();

        spdlog::set_pattern(DEFAULT_PATTERN);
        spdlog::set_level(spdlog::level::trace);

        std::shared_ptr<spdlog::logger> engineLogger = CreateLogger(ENGINE_LOGGER_NAME);
        std::shared_ptr<spdlog::logger> appLogger = CreateLogger(appName);

        _appLoggerName = appName;
    }

    std::shared_ptr<spdlog::logger> Logging::CreateLogger(const std::string& loggerName)
    {
        ANE_PROFILE_FUNCTION();

        auto defaultSink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
        auto newLogger = std::make_shared<spdlog::logger>(loggerName, std::move(defaultSink));
        auto loggerSink = std::make_shared<LogSink>();
        loggerSink->BindSink(MakeDelegate(&Logging::OnSink));
        newLogger->sinks().push_back(std::move(loggerSink));

        spdlog::details::registry::instance().initialize_logger(newLogger);
        _registeredLoggers.push_back(loggerName);

        return newLogger;
    }

    spdlog::logger& Logging::GetEngineLogger()
    {
        return GetLogger(ENGINE_LOGGER_NAME);
    }

    spdlog::logger& Logging::GetAppLogger()
    {
        return GetLogger(_appLoggerName);
    }

    spdlog::logger& Logging::GetLogger(std::string const& name)
    {
        auto logger = spdlog::get(name);
        if(logger == nullptr)
        {
            logger = CreateLogger(name);
        }
        return *logger;
    }

    const std::vector<std::string>& Logging::GetRegisteredLoggerNames()
    {
        return _registeredLoggers;
    }

    const std::string& Logging::GetRegisteredLoggerName(const LoggerIndex loggerId)
    {
        if (loggerId >= _registeredLoggers.size()) return "unknown";
        return _registeredLoggers[loggerId];
    }

    LoggerIndex Logging::GetRegisteredLoggerIndex(std::string const& name)
    {
        if (const auto it = std::ranges::find(_registeredLoggers, name); it != _registeredLoggers.cend())
        {
            return std::distance(_registeredLoggers.begin(), it);
        }
        return 0;
    }

    const std::list<LogMessage>& Logging::GetMessages()
    {
        return _logMessages;
    }

    void Logging::ClearMessages()
    {
        _logMessages.clear();
    }

    void Logging::OnSink(const log_msg& logMsg)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        if(_logMessages.size() >= 100)
        {
            _logMessages.pop_back();
        }

        LogMessage newLogMessage(logMsg);

        _writer.WriteToFile(newLogMessage);

        _logMessages.push_front(newLogMessage);
    }
}
