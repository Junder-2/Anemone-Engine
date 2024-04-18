#include "anepch.h"
#include "LogSink.h"

namespace Engine
{
    void LogSink::BindSink(DelegateFree<void(const spdlog::details::log_msg&)> delegateMember)
    {
        _sinkDelegate = delegateMember;
    }

    void LogSink::sink_it_(const spdlog::details::log_msg& msg)
    {
        if(_sinkDelegate) _sinkDelegate(msg);
    }

    void LogSink::flush_()
    {

    }
}
