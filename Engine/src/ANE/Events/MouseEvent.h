#pragma once
#include "Event.h"
#include "ANE/Input/MouseInputAction.h"

namespace Engine
{
    /**
    * Mouse event, stores the new (x,y) and deltas
    */
    class MouseMovementEvent final : public Event
    {
    public:
        MouseMovementEvent(const MouseMoveValue& value) : _moveValue(value) {}

        EventType GetEventType() const override { return EventType::MouseMovement; }
        int GetEventCategories() const override { return (EventCategoryInput | EventCategoryMouse); }

        MouseMoveValue GetInputValue() const { return _moveValue; }

    private:
        MouseMoveValue _moveValue;
    };

    /**
    * Mouse event, stores the new trigger value of all buttons
    */
    class MouseButtonEvent final : public Event
    {
    public:
        MouseButtonEvent(const MouseButtonValues buttonValues) : _buttonValues(buttonValues) {}

        EventType GetEventType() const override { return EventType::MouseButton; }
        int GetEventCategories() const override { return (EventCategoryInput | EventCategoryMouse); }

        MouseButtonValues GetInputValue() const { return _buttonValues; }

    private:
        MouseButtonValues _buttonValues;
    };

    /**
    * Mouse event, stores the new scroll deltas
    */
    class MouseScrollEvent final : public Event
    {
    public:
        MouseScrollEvent(const float xDelta, const float yDelta) : _xDelta(xDelta), _yDelta(yDelta) {}
        MouseScrollEvent(const Vector2 delta) : _xDelta(delta.X), _yDelta(delta.Y) {}

        EventType GetEventType() const override { return EventType::MouseScrolled; }
        int GetEventCategories() const override { return (EventCategoryInput | EventCategoryMouse); }

        float GetXDelta() const { return _xDelta; }
        float GetYDelta() const { return _yDelta; }

    private:
        float _xDelta;
        float _yDelta;
    };
}
