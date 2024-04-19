#include "anepch.h"
#include "LogFileWriter.h"
#include "ANE/Core/Entity/UUIDGenerator.h"


void Engine::LogFileWriter::InitLogWriter()
{
    _sessionUUID = UUIDGenerator::get_uuid();
}

void Engine::LogFileWriter::WriteToFile(log_msg& msg)
{
    if (_sessionUUID != _currentLogUUID)
    {
        _currentLogUUID = _sessionUUID;

        // generate new file
    }

    // append message to log
}
