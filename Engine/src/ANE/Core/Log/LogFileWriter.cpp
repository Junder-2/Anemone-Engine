#include "anepch.h"
#include "LogFileWriter.h"
#include <iostream>
#include <filesystem>
#include "ANE/Core/Entity/UUIDGenerator.h"
#include "ANE/Utilities/LoggingUtilities.h"

namespace Engine
{
    int LogFileWriter::DirectoryCount() const
    {
        int fileCount = 0;

        for (const auto& entry : std::filesystem::directory_iterator(path))
            if (is_regular_file(entry.path())) ++fileCount;

        return fileCount;
    }


    void Engine::LogFileWriter::WriteToFile(LogMessage& msg)
    {
        //todo: this should probably prevent the call altogether
        if (msg.LevelCategory == LogLevelCategory::LevelDebug || msg.LevelCategory == LogLevelCategory::LevelNone || msg.LevelCategory == LogLevelCategory::LevelInfo) return;

        if (fileName.empty())
        {
            ANE_ELOG("We ARE NOT EMPTY");
            const auto now = std::chrono::system_clock::now();
            const auto time = std::chrono::system_clock::to_time_t(now);

            std::tm tm;

            #ifdef _WIN32
            const auto err = localtime_s(&tm, &time); // couldn't get all info on the error codes from this, see if and when assert fails
            //ANE_ASSERT(err == 0, "Unable to get local time");
            #else
        localtime_r(&time, &tm);
            #endif

            std::stringstream ss;
            ss << std::put_time(&tm, "%Y-%m-%d_%H%M%S");

            const std::string timeDate = ss.str();
            fileName = path + "LOG_" + timeDate + ".txt";

            outputFile.open(fileName, std::ios::app);
        }

        // ANE_ASSERT(!outputFile.is_open(), "No open logfile");

        std::string messageBuilder;

        const std::string levelName = LoggingUtilities::ToString(msg.LevelCategory);

        messageBuilder.append(std::format("[{0} {1}", msg.LoggerName, levelName));

        messageBuilder.append(std::format(" {0}] ", msg.Time));

        messageBuilder.append(std::format("[{0}] ", msg.Source));

        messageBuilder.append(msg.Message);

        outputFile << messageBuilder << '\n';
    }

    Engine::LogFileWriter::~LogFileWriter()
    {
        outputFile.close();
    }
}
