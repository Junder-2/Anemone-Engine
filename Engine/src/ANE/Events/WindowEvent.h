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
        WindowResizeEvent(const uint width, const uint height, const uint windowIndex)
            : _width(width), _height(height), _windowIndex(windowIndex) {}

        EventType GetEventType() const override { return EventType::WindowResize; }
        int GetEventCategories() const override { return (EventCategoryWindow); }

        uint GetWidth() const { return _width; }
        uint GetHeight() const { return _height; }

        uint GetWindowIndex() const { return _windowIndex; }
        bool IsMainWindow() const;
        bool IsActiveViewport() const;

    private:
        uint _width;
        uint _height;
        uint _windowIndex;
    };

    /**
    * Window event, stores the new (x,y)
    */
    class WindowMovedEvent final : public Event
    {
    public:
        WindowMovedEvent(const uint x, const uint y, const uint windowIndex)
            : _xPos(x), _yPos(y), _windowIndex(windowIndex) {}

        EventType GetEventType() const override { return EventType::WindowMoved; }
        int GetEventCategories() const override { return (EventCategoryWindow); }

        uint GetX() const { return _xPos; }
        uint GetY() const { return _yPos; }

        uint GetWindowIndex() const { return _windowIndex; }
        bool IsMainWindow() const;
        bool IsActiveViewport() const;

    private:
        uint _xPos;
        uint _yPos;
        uint _windowIndex;
    };
}
