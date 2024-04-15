#include "anepch.h"
#include "Logging.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Engine
{
    std::shared_ptr<spdlog::logger> Logging::_engineLogger;
    std::shared_ptr<spdlog::logger> Logging::_appLogger;

    void Logging::InitEngine()
    {
        #ifndef ANE_DIST
        if(_engineLogger != nullptr) return;
        spdlog::set_pattern("%^[%n %L %T] [%s:%#] %v%$");

        _engineLogger = spdlog::stdout_color_mt("ENGINE");
        _engineLogger->set_level(spdlog::level::trace);
        #endif
    }

    void Logging::InitApp(const std::string& appName)
    {
        #ifndef ANE_DIST
        if(GetEngineLogger() == nullptr || _appLogger != nullptr) return;
        _appLogger = spdlog::stdout_color_mt(appName);
        _appLogger->set_level(spdlog::level::trace);
        #endif
    }

    std::shared_ptr<spdlog::logger>& Logging::GetEngineLogger()
    {
        if(_engineLogger == nullptr)
        {
            // We have to use built in as normal logger doesnt exist yet
            SPDLOG_ERROR("Engine logger not yet initialized");
            ANE_DEBUG_BREAK();
        }
        return _engineLogger;
    }

    std::shared_ptr<spdlog::logger>& Logging::GetAppLogger()
    {
        ANE_EASSERT(_appLogger == nullptr, "App logger not yet initialized");
        return _appLogger;
    }
}
