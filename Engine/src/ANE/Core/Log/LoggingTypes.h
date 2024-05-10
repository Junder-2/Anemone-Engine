#pragma once
#include "ANE/Utilities/CommonUtilities.h"

namespace Engine
{
    typedef uint8_t LogLevelCategories;

    enum class LogLevelCategory : uint8_t
    {
        None = 0,
        Trace = BIT(0),
        Debug = BIT(1),
        Info = BIT(2),
        Warn = BIT(3),
        Error = BIT(4)
    };
    ENUM_CLASS_OPERATORS(LogLevelCategory);

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
