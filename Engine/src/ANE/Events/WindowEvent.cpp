#include "anepch.h"
#include "WindowEvent.h"

#include "ANE/Core/Application.h"
#include "ANE/Core/Window.h"

namespace Engine
{
    bool WindowResizeEvent::IsMainWindow() const
    {
        return Application::Get().GetWindow().GetWindowProperties().Id == _windowIndex;
    }

    bool WindowResizeEvent::IsActiveViewport() const
    {
        return Application::Get().GetWindow().GetViewportId() == _windowIndex;
    }

    bool WindowMovedEvent::IsMainWindow() const
    {
        return Application::Get().GetWindow().GetWindowProperties().Id == _windowIndex;
    }

    bool WindowMovedEvent::IsActiveViewport() const
    {
        return Application::Get().GetWindow().GetViewportId() == _windowIndex;
    }
}
