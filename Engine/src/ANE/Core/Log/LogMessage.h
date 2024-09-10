#pragma once

namespace Engine
{
    using log_msg = spdlog::details::log_msg;

    //log_msg
    struct LogMessage
    {
        std::string SegmentedMessage;

        LoggerIndex LoggerNameIndex;
        LogLevelCategory LevelCategory;
        uint16 SourceIndex;
        uint16 MessageIndex;

        LogMessage() = default;
        LogMessage(const log_msg& logMsg);
        LogMessage(const LogMessage& other) = default;
        LogMessage& operator=(const LogMessage& other) = default;

        std::string ConstructMessage(bool showLoggerName, bool showLevel, bool showTime, bool showSource) const;

        std::string RetrieveLoggerName() const;

        std::string RetrieveLevel() const;

        std::string RetrieveTime() const;

        std::string RetrieveSource() const;

        std::string RetrieveMessage() const;
    };
}
