﻿#include "anepch.h"
#include "LogFileWriter.h"
#include <filesystem>
#include <string>

#include "LogMessage.h"
#include "ANE/Utilities/LoggingUtilities.h"

namespace Engine
{
    int LogFileWriter::DirectoryCount() const
    {
        ANE_PROFILE_FUNCTION();

        int fileCount = 0;

        for (const auto& entry : std::filesystem::directory_iterator(_path))
            if (is_regular_file(entry.path())) ++fileCount;

        return fileCount;
    }

    void LogFileWriter::GetOldestFile(std::filesystem::path& oldestFile) const
    {
        ANE_PROFILE_FUNCTION();

        std::filesystem::file_time_type oldestTime = std::filesystem::file_time_type::max();
        for (const auto& entry : std::filesystem::directory_iterator(_path))
        {
            if (!is_regular_file(entry)) continue;
            auto lastWriteTime = last_write_time(entry);
            if ( lastWriteTime >= oldestTime) continue;

            oldestTime = lastWriteTime;
            oldestFile = entry.path();
        }
    }

    std::string LogFileWriter::ConstructMessage(const LogMessage& msg)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        std::string messageBuilder = msg.ConstructMessage(true, true, true, true);

        return messageBuilder;
    }

    void LogFileWriter::WriteToFile(const LogMessage& msg)
    {
        ANE_DEEP_PROFILE_FUNCTION();

        if (msg.LevelCategory == LogLevelCategory::Debug || msg.LevelCategory == LogLevelCategory::None || msg.LevelCategory == LogLevelCategory::Info) return;

        if (!std::filesystem::is_directory(_path)) std::filesystem::create_directory(_path);

        if (_fileName.empty())
        {
            if (DirectoryCount() >= MAX_FILE_COUNT)
            {
                std::filesystem::path oldestFile;

                GetOldestFile(oldestFile);

                if (!oldestFile.empty()) std::filesystem::remove(oldestFile);
            }

            const auto now = std::chrono::system_clock::now();
            const auto time = std::chrono::system_clock::to_time_t(now);

            std::tm tm;

            #ifdef _WIN32
            const auto err = localtime_s(&tm, &time); // couldn't get all info on the error codes from this, see if and when assert fails
            ANE_ASSERT(err == 0, "Unable to get local time");
            #else
            localtime_r(&time, &tm);
            #endif


            std::stringstream ss;
            ss << std::put_time(&tm, "%Y-%m-%d_%H%M%S");

            const std::string timeDate = ss.str();
            _fileName = _path + "LOG_" + timeDate + ".txt";

            _outputFile.open(_fileName, std::ios::app);
        }

        ANE_ASSERT(_outputFile.is_open(), "No open logfile");

        _outputFile << ConstructMessage(msg) << '\n';
    }

    LogFileWriter::~LogFileWriter()
    {
        _outputFile.close();
    }
}
