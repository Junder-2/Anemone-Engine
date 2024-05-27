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
        InputFlush, KeyboardInput,
        MouseButton, MouseMovement, MouseScrolled
    };

    /**
    * The category of event, used to filter events
    */
    typedef enum
    {
        EventCategoryNone = 0,
        EventCategoryWindow = BIT(0),
        EventCategoryInput = BIT(1),
        EventCategoryKeyboard = BIT(2),
        EventCategoryMouse = BIT(3),
    } EventCategory;

    typedef enum : uint8
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
        bool IsConsumed() const { return _consumed && !_isFlush; }

        void MarkFlush() { _isFlush = true; }
        bool IsFlush() const { return _isFlush; }

    private:
        bool _consumed = false;
        bool _isFlush = false;
    };
}

#include "WindowEvent.h"
#include "KeyboardEvent.h"
#include "MouseEvent.h"
