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

    typedef enum
    {
        MouseButtonLeft = 0,
        MouseButtonMiddle = 1,
        MouseButtonRight = 3,
        MouseButtonX1 = 4,
        MouseButtonX2 = 5,
    } MouseButton;

    #define MOUSE_BUTTON_MAX (MouseButtonX2+1)
    #define MOUSE_BUTTON_TO_SDL_MOUSE_BUTTON(x) ((x)-1)

    typedef enum {
        MouseButtonMaskLeft = 0,
        MouseButtonMaskMiddle = TWO_BIT_MASK(MouseButtonMiddle),
        MouseButtonMaskRight = TWO_BIT_MASK(MouseButtonRight),
        MouseButtonMaskX1 = TWO_BIT_MASK(MouseButtonX1),
        MouseButtonMaskX2 = TWO_BIT_MASK(MouseButtonX2),
    } MouseButtonMask;

    inline MouseButtonMask GetMouseButtonMaskFromMouseButton(const int button)
    {
        return static_cast<MouseButtonMask>(TWO_BIT_MASK(button));
    }

    inline TriggerState GetTriggerStateFromMouseButtonState(const uint16_t state, const int button)
    {
        return static_cast<TriggerState>((state & TWO_BIT_MASK(button)) >> 2*button);
    }

    inline uint16_t GetMouseButtonStateFromMouseButton(const TriggerState state, const int button)
    {
        return state << button*2;
    }

}
