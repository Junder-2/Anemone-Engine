#pragma once
#include "ANE/Input/InputTypes.h"
#include "ANE/Subsystem/SubsystemCollection.h"

namespace Engine
{
    namespace InputUtilities
    {
        inline std::string ToString(const InputDeviceType type)
        {
            switch (type)
            {
                case InputDeviceKeyboard: return "Keyboard";
                case InputDeviceGamepad: return "Gamepad";
                case InputDeviceMouse: return "Mouse";
                default: return "Unknown";
            }
        }

        inline std::string ToString(const TriggerState type)
        {
            switch (type)
            {
                case TriggerNone: return "None";
                case TriggerStarted: return "Started";
                case TriggerHolding: return "Holding";
                case TriggerStopped: return "Stopped";
                default: return "Unknown";
            }
        }

        inline std::string ToString(const MouseButton type)
        {
            switch (type)
            {
                case MouseButtonLeft: return "MLeft";
                case MouseButtonMiddle: return "MMiddle";
                case MouseButtonRight: return "MRight";
                case MouseButtonX1: return "MX1";
                case MouseButtonX2: return "MX2";
                default: return "Unknown";
            }
        }
    }
}
