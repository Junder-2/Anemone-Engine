#pragma once
#include "Event.h"
#include "ANE/Input/InputAction.h"

namespace Engine
{
    /**
    * Keyboard event, stores the trigger state
    */
    class KeyboardKeyEvent final : public Event
    {
    public:
        KeyboardKeyEvent(const InputValue inputValue) : _inputValue(inputValue) {}

        EventType GetEventType() const override { return EventType::KeyboardInput; }
        int GetEventCategories() const override { return (EventCategoryInput | EventCategoryKeyboard); }

        InputValue GetInputValue() const { return _inputValue; }

    private:
        InputValue _inputValue;
    };

    /**
    * Input Event, tells listeners to zero any inputs
    */
    class InputFlushEvent final : public Event
    {
    public:
        InputFlushEvent() { MarkFlush(); }

        EventType GetEventType() const override { return EventType::InputFlush; }
        int GetEventCategories() const override { return (EventCategoryInput); }
    };

}
