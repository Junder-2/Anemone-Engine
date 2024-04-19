#pragma once

namespace Engine
{
    namespace LoggingUtilities
    {
        inline std::string ToString(const LogLevelCategory type)
        {
            switch (type)
            {
                case LogLevelCategory::LevelTrace: return "Trace";
                case LogLevelCategory::LevelDebug: return "Debug";
                case LogLevelCategory::LevelInfo: return "Info";
                case LogLevelCategory::LevelWarn: return "Warn";
                case LogLevelCategory::LevelError: return "Error";
                default: return "Unknown";
            }
        }
    }
}
