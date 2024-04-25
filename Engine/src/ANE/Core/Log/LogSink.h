#pragma once

#include <spdlog/common.h>
#include <spdlog/details/log_msg.h>

#include "ANE/Delegate/Delegate.h"
#include "spdlog/sinks/base_sink.h"

namespace Engine
{
    using log_msg = spdlog::details::log_msg;

    class LogSink final : public spdlog::sinks::base_sink<std::mutex>
    {
    public:
        void BindSink(const Delegate<void(const log_msg&)>& delegate);

    private:
        void sink_it_(const log_msg &msg) override;
        void flush_() override;

        SinglecastDelegate<void(const log_msg&)> _sinkDelegate;
    };
}
