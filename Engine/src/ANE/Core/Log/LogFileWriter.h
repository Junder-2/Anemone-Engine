#pragma once
#include <fstream>
#include "LoggingTypes.h"

namespace Engine
{
    class Logging;

    class LogFileWriter
    {
    public:
        int DirectoryCount() const;
        void WriteToFile(LogMessage& msg);

    private:
        std::ofstream outputFile;
        std::string fileName;

        ~LogFileWriter();
        LogFileWriter() = default;

        const std::string path = "../Engine/Logs/";

        const int MaxFileCount = 3;

        friend Engine::Logging;
    };
}
