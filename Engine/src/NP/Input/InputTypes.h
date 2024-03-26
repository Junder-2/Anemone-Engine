#pragma once

namespace Engine
{
    typedef enum
    {
        Trigger,
        Axis
    } InputValueType;

    typedef enum
    {
        TriggerNone,
        TriggerStarted,
        TriggerHolding,
        TriggerStopped,
    } TriggerState;

}
