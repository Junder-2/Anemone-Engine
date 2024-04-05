#pragma once
#include "ANE/Utilities/CommonUtilities.h"

namespace Engine
{
    /**
    * The type of event, used to determine the derived class Event is
    */
    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowStateChange, WindowFocusChange, WindowMoved,
        KeyboardInput,
        MouseButton, MouseMovement, MouseScrolled
    };

    /**
    * The category of event, used to filter events
    */
    typedef enum
    {
        NoneEvent = 0,
        WindowEvent = BIT(0),
        InputEvent = BIT(1),
        KeyboardEvent = BIT(2),
        MouseEvent = BIT(3),
    } EventCategory;

    typedef enum : uint8_t
    {
        WindowMinimized = 0,
        WindowRestored = 1,
        WindowMaximized = 2,
    } WindowState;

    /**
    * Base class for events, use type to determine the derived class
    */
    class Event
    {
    public:
        virtual ~Event() = default;
        virtual EventType GetEventType() const = 0;
        virtual int GetEventCategories() const = 0;

        bool HasCategory(const EventCategory category) const { return GetEventCategories() & category; }

        void Consume() { _consumed = true; }
        bool IsConsumed() const { return _consumed; }

    private:
        bool _consumed = false;
    };
}
