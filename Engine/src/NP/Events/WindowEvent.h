#pragma once
#include "Event.h"

namespace Engine
{
    class WindowCloseEvent final : public Event
    {
    public:
        WindowCloseEvent() = default;

        EventType GetEventType() const override { return EventType::WindowClose; }
        int GetEventCategories() const override { return (WindowEvent); }
    };

    class WindowFocusChangeEvent final : public Event
    {
    public:
        WindowFocusChangeEvent(bool isFocused) : _isFocused(isFocused) {}

        EventType GetEventType() const override { return EventType::WindowFocusChange; }
        int GetEventCategories() const override { return (WindowEvent); }

        bool IsFocused() const { return _isFocused; }

    private:
        bool _isFocused;
    };

    class WindowResizeEvent final : public Event
    {
    public:
        WindowResizeEvent(const uint32_t width, const uint32_t height) : _width(width), _height(height) {}

        EventType GetEventType() const override { return EventType::WindowResize; }
        int GetEventCategories() const override { return (WindowEvent); }

        uint32_t GetWidth() const { return _width; }
        uint32_t GetHeight() const { return _height; }

    private:
        uint32_t _width;
        uint32_t _height;
    };

    class WindowMovedEvent final : public Event
    {
    public:
        WindowMovedEvent(const uint32_t x, const uint32_t y, const float xDelta, const float yDelta) : _xPos(x), _yPos(y), _xDelta(xDelta), _yDelta(yDelta) {}

        EventType GetEventType() const override { return EventType::WindowMoved; }
        int GetEventCategories() const override { return (WindowEvent); }

        uint32_t GetX() const { return _xPos; }
        uint32_t GetY() const { return _yPos; }

        float GetXDelta() const { return _xDelta; }
        float GetYDelta() const { return _yDelta; }

    private:
        uint32_t _xPos;
        uint32_t _yPos;

        float _xDelta;
        float _yDelta;
    };
}
