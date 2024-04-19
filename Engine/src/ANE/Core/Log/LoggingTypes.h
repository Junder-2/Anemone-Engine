#pragma once
#include "ANE/Utilities/CommonUtilities.h"

namespace Engine
{
    // TODO: Talk about log levels and decide on what we want
    enum class LogLevelCategory
    {
        LevelNone = 0,
        LevelInfo = BIT(0),
        LevelWarn = BIT(1),
        LevelError = BIT(2)
    };

    //log_msg
    struct LogMessage
    {
        std::string Time;
        std::string Source;
        std::string LoggerName;
        LogLevelCategory LevelCategory;
        std::string Message;
    };
}
