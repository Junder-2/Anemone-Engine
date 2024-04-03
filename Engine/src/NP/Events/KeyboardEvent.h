#pragma once
#include "Event.h"
#include "../Input/InputAction.h"

namespace Engine
{
    class KeyTriggerEvent final : public Event
    {
    public:
        KeyTriggerEvent(const InputValue inputValue) : _inputValue(inputValue) {}

        EventType GetEventType() const override { return EventType::KeyboardInput; }
        int GetEventCategories() const override { return (InputEvent | KeyboardEvent); }

        operator const InputValue&() const { return _inputValue; }
        operator InputValue&() { return _inputValue; }

    private:
        InputValue _inputValue;
    };

    class KeyAxisEvent final : public Event
    {
    public:
        KeyAxisEvent(const InputValue inputValue) : _inputValue(inputValue) {}

        EventType GetEventType() const override { return EventType::KeyboardInput; }
        int GetEventCategories() const override { return (InputEvent | KeyboardEvent); }

        operator const InputValue&() const { return _inputValue; }
        operator InputValue&() { return _inputValue; }

    private:
        InputValue _inputValue;
    };
}
