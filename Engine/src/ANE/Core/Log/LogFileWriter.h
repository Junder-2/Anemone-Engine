#pragma once
#include <fstream>
#include "LoggingTypes.h"

namespace Engine
{
    class Logging;

    class LogFileWriter
    {
    public:
        void GetOldestFile(std::filesystem::path& oldestFile) const;
        void WriteToFile(LogMessage& msg);

    private:
        [[nodiscard("Return value not used.")]] int DirectoryCount() const;
        [[nodiscard("Return value not used.")]] static std::string ConstructMessage(LogMessage& msg);

        std::ofstream _outputFile;
        std::string _fileName;

        ~LogFileWriter();
        LogFileWriter() = default;

        const int MAX_FILE_COUNT = 4;
        const std::string _path = "../Engine/Logs/";

        friend Logging;
    };
}
