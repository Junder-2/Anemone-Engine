#pragma once

namespace Engine
{
    class SubSystem
    {
    public:
        SubSystem() = default;
        virtual ~SubSystem() = default;
        virtual void Free() { delete this; }
    };
}
