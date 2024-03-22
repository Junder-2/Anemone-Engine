#include "Logging.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Engine
{
    std::shared_ptr<spdlog::logger> Logging::engineLogger;
    std::shared_ptr<spdlog::logger> Logging::appLogger;
    
    void Logging::Init()
    {
        spdlog::set_pattern("%^[%n %T] %v%$");
        
        engineLogger = spdlog::stdout_color_mt("ENGINE");
        engineLogger->set_level(spdlog::level::trace);

        appLogger = spdlog::stdout_color_mt("APP");
        appLogger->set_level(spdlog::level::trace);
    }
}
