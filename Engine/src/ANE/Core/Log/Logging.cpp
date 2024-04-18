#include "anepch.h"
#include "Logging.h"

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

    static spdlog::pattern_formatter LoggerNameFormatter(LOGGER_NAME_PATTERN);
    static spdlog::pattern_formatter SourceFormatter(SOURCE_PATTERN);
    static spdlog::pattern_formatter TimeFormatter(TIME_PATTERN);

    void Logging::Init(const std::string& appName)
    {
        spdlog::set_pattern(DEFAULT_PATTERN);
        spdlog::set_level(spdlog::level::trace);

        std::shared_ptr<spdlog::logger> engineLogger = CreateLogger(ENGINE_LOGGER_NAME);
        std::shared_ptr<spdlog::logger> appLogger = CreateLogger(appName);

        _appLoggerName = appName;
    }

    std::shared_ptr<spdlog::logger> Logging::CreateLogger(const std::string& loggerName)
    {
        auto defaultSink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>();
        auto newLogger = std::make_shared<spdlog::logger>(loggerName, std::move(defaultSink));
        auto loggerSink = std::make_shared<LogSink>();
        loggerSink->BindSink(MakeDelegate(&Logging::OnSink));
        newLogger->sinks().push_back(std::move(loggerSink));

        spdlog::details::registry::instance().initialize_logger(newLogger);

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

    const std::list<LogMessage>& Logging::GetMessages()
    {
        return _logMessages;
    }

    void Logging::OnSink(const log_msg& logMsg)
    {
        if(_logMessages.size() >= 100)
        {
            _logMessages.pop_back();
        }

        LogMessage newLogMessage;

        LoggerNameFormatter.format(logMsg, newLogMessage.LoggerName);
        newLogMessage.LoggerName.erase(newLogMessage.LoggerName.length()-2); //spdlog appends linebreaks

        TimeFormatter.format(logMsg, newLogMessage.Time);
        newLogMessage.Time.erase(newLogMessage.Time.length()-2);

        SourceFormatter.format(logMsg, newLogMessage.Source);
        newLogMessage.Source.erase(newLogMessage.Source.length()-2);

        newLogMessage.Message = logMsg.payload;
        newLogMessage.Level = logMsg.level;

        _logMessages.push_front(newLogMessage);
    }
}
