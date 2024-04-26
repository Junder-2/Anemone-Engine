#include "anepch.h"
#include "LogSink.h"

namespace Engine
{
    void LogSink::BindSink(const Delegate<void(const log_msg&)>& delegate)
    {
        _sinkDelegate = delegate;
    }

    void LogSink::sink_it_(const spdlog::details::log_msg& msg)
    {
        if(_sinkDelegate) _sinkDelegate(msg);
    }

    void LogSink::flush_()
    {

    }
}
