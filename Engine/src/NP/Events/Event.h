#pragma once
#include "../Utilities/CommonUtilities.h"

namespace Engine
{
    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocusChange, WindowMoved,
        KeyboardInput,
        MouseButton, MouseMovement, MouseScrolled
    };

    typedef enum
    {
        None = 0,
        WindowEvent = BIT(0),
        InputEvent = BIT(0),
        KeyboardEvent = BIT(1),
        MouseEvent = BIT(2),
    } EventCategory;

    class Event
    {
    public:
        virtual ~Event() = default;
        virtual EventType GetEventType() const = 0;
        virtual int GetEventCategories() const = 0;

        bool HasCategory(EventCategory category) const { return GetEventCategories() & category; }

        void Consume() { _consumed = true; }
        bool IsConsumed() const { return _consumed; }

    private:
        bool _consumed = false;
    };
}
