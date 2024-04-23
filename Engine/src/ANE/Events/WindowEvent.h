#pragma once
#include "Event.h"

namespace Engine
{
    /**
    * Window event
    */
    class WindowCloseEvent final : public Event
    {
    public:
        WindowCloseEvent() = default;

        EventType GetEventType() const override { return EventType::WindowClose; }
        int GetEventCategories() const override { return (EventCategoryWindow); }
    };

    /**
    * Window event, stores the new window state (minimized, restored, maximized)
    */
    class WindowStateChangeEvent final : public Event
    {
    public:
        WindowStateChangeEvent(const WindowState state) : _stateValue(state) {}

        EventType GetEventType() const override { return EventType::WindowStateChange; }
        int GetEventCategories() const override { return (EventCategoryWindow); }

        WindowState GetState() const { return _stateValue; }

    private:
        WindowState _stateValue;
    };

    /**
    * Window event, stores the focus
    */
    class WindowFocusChangeEvent final : public Event
    {
    public:
        WindowFocusChangeEvent(const bool isFocused) : _isFocused(isFocused) {}

        EventType GetEventType() const override { return EventType::WindowFocusChange; }
        int GetEventCategories() const override { return (EventCategoryWindow); }

        bool IsFocused() const { return _isFocused; }

    private:
        bool _isFocused;
    };

    /**
    * Window event, stores new width and height
    */
    class WindowResizeEvent final : public Event
    {
    public:
        WindowResizeEvent(const uint32_t width, const uint32_t height) : _width(width), _height(height) {}

        EventType GetEventType() const override { return EventType::WindowResize; }
        int GetEventCategories() const override { return (EventCategoryWindow); }

        uint32_t GetWidth() const { return _width; }
        uint32_t GetHeight() const { return _height; }

    private:
        uint32_t _width;
        uint32_t _height;
    };

    /**
    * Window event, stores the new (x,y) and deltas
    */
    class WindowMovedEvent final : public Event
    {
    public:
        WindowMovedEvent(const uint32_t x, const uint32_t y) : _xPos(x), _yPos(y) {}

        EventType GetEventType() const override { return EventType::WindowMoved; }
        int GetEventCategories() const override { return (EventCategoryWindow); }

        uint32_t GetX() const { return _xPos; }
        uint32_t GetY() const { return _yPos; }

    private:
        uint32_t _xPos;
        uint32_t _yPos;
    };
}
