#pragma once
#include "Event.h"
#include "../Input/MouseInputAction.h"

namespace Engine
{
    class MouseMovementEvent final : public Event
    {
    public:
        MouseMovementEvent(const MouseMoveValue value) : _moveValue(value) {}

        EventType GetEventType() const override { return EventType::MouseMovement; }
        int GetEventCategories() const override { return (InputEvent | MouseEvent); }

        operator const MouseMoveValue&() const { return _moveValue; }
        operator MouseMoveValue&() { return _moveValue; }

    private:
        MouseMoveValue _moveValue;
    };

    class MouseButtonEvent final : public Event
    {
    public:
        MouseButtonEvent(const MouseButtonValues buttonValues) : _buttonValues(buttonValues) {}

        EventType GetEventType() const override { return EventType::MouseButton; }
        int GetEventCategories() const override { return (InputEvent | MouseEvent); }

        operator const MouseButtonValues&() const { return _buttonValues; }
        operator MouseButtonValues&() { return _buttonValues; }

    private:
        MouseButtonValues _buttonValues;
    };

    class MouseScrollEvent final : public Event
    {
    public:
        MouseScrollEvent(const float xDelta, const float yDelta) : _xDelta(xDelta), _yDelta(yDelta) {}
        MouseScrollEvent(const glm::vec2 delta) : _xDelta(delta.x), _yDelta(delta.y) {}

        EventType GetEventType() const override { return EventType::MouseScrolled; }
        int GetEventCategories() const override { return (InputEvent | MouseEvent); }

        float GetXDelta() const { return _xDelta; }
        float GetYDelta() const { return _yDelta; }

    private:
        float _xDelta;
        float _yDelta;
    };
}
