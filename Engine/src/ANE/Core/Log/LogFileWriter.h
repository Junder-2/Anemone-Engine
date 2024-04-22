#pragma once

namespace Engine
{
    class LogFileWriter
    {
    public:
        void InitLogWriter();
        void WriteToFile(log_msg& msg);

    private:
        std::string _sessionUUID;
        std::string _currentLogUUID;


    };
}
