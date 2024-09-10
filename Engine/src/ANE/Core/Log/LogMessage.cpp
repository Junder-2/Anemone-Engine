#include "anepch.h"
#include "LogMessage.h"

#include "ANE/Utilities/LoggingUtilities.h"

namespace Engine
{
    LogMessage::LogMessage(const log_msg& logMsg)
    {
        std::string tempString;
        SegmentedMessage.clear();

        Logging::LoggerNameFormatter.format(logMsg, tempString);
        tempString.erase(tempString.length()-2); //spdlog appends linebreaks

        LoggerNameIndex = Logging::GetRegisteredLoggerIndex(tempString);

        tempString.clear();
        Logging::TimeFormatter.format(logMsg, tempString);
        tempString.erase(tempString.length()-2);

        SegmentedMessage.append(tempString);

        tempString.clear();
        Logging::SourceFormatter.format(logMsg, tempString);
        tempString.erase(tempString.length()-2);

        SourceIndex = SegmentedMessage.length();
        SegmentedMessage.append(tempString);

        MessageIndex = SegmentedMessage.length();
        SegmentedMessage.append(logMsg.payload);

        switch (logMsg.level)
        {
            case spdlog::level::trace:
                LevelCategory = LogLevelCategory::Trace;
            break;
            case spdlog::level::debug:
                LevelCategory = LogLevelCategory::Debug;
            break;
            case spdlog::level::info:
                LevelCategory = LogLevelCategory::Info;
            break;
            case spdlog::level::warn:
                LevelCategory = LogLevelCategory::Warn;
            break;
            case spdlog::level::err:
            case spdlog::level::critical:
                LevelCategory = LogLevelCategory::Error;
            break;
            default:
                LevelCategory = LogLevelCategory::None;
            break;
        }
    }

    std::string LogMessage::ConstructMessage(const bool showLoggerName, const bool showLevel, const bool showTime, const bool showSource) const
    {
        // Right now using append, from searches ostream is slower?
        std::string fullMessage;
        if(showLoggerName && showLevel)
        {
            fullMessage.append(std::format("[{0} {1}", RetrieveLoggerName(), RetrieveLevel()));
        }
        else if(showLoggerName)
        {
            fullMessage.append(std::format("[{0}", RetrieveLoggerName()));
        }
        else if(showLevel)
        {
            fullMessage.append(std::format("[{0}", RetrieveLevel()));
        }

        if(showTime && (showLevel || showLoggerName))
        {
            fullMessage.append(std::format(" {0}] ", RetrieveTime()));
        }
        else if(showTime)
        {
            fullMessage.append(std::format("[{0}] ", RetrieveTime()));
        }
        else if((showLevel || showLoggerName))
        {
            fullMessage.append("] ");
        }

        if(showSource)
        {
            fullMessage.append(std::format("[{0}] ", RetrieveSource()));
        }

        fullMessage.append(RetrieveMessage());

        return fullMessage;
    }

    std::string LogMessage::RetrieveLoggerName() const
    {
        return LoggingUtilities::ToLoggerName(LoggerNameIndex);
    }

    std::string LogMessage::RetrieveLevel() const
    {
        return LoggingUtilities::ToString(LevelCategory);
    }

    std::string LogMessage::RetrieveTime() const
    {
        return SegmentedMessage.substr(0, SourceIndex);
    }

    std::string LogMessage::RetrieveSource() const
    {
        return SegmentedMessage.substr(SourceIndex, MessageIndex - SourceIndex);
    }

    std::string LogMessage::RetrieveMessage() const
    {
        return SegmentedMessage.substr(MessageIndex, SegmentedMessage.length() - SourceIndex);
    }
}
