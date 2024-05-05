#include "anepch.h"
#include "EventHandler.h"

#include "Event.h"
#include "ANE/Core/Application.h"
#include "ANE/Core/Window.h"

namespace Engine
{
    void EventHandler::ConsumeEvent()
    {
        ANE_DEEP_PROFILE_FUNCTION();

        if(_currentEvent == nullptr)
        {
            ANE_ELOG_WARN("Cannot consume event outside of event handling");
            return;
        }
        _currentEvent->Consume();
    }

    void EventHandler::DispatchEditorEvents()
    {
        ANE_DEEP_PROFILE_FUNCTION();

        if(_currentEvent == nullptr)
        {
            ANE_ELOG_WARN("Cannot execute event outside of event handling");
            return;
        }

        if(_currentEvent->IsConsumed()) return;

        if(_editorEventDelegate) _editorEventDelegate(*_currentEvent);
    }

    void EventHandler::DispatchAppEvents()
    {
        ANE_DEEP_PROFILE_FUNCTION();

        if(_currentEvent == nullptr)
        {
            ANE_ELOG_WARN("Cannot execute event outside of event handling");
            return;
        }

        if(_currentEvent->IsConsumed()) return;

        if(_blockAllAppEvents)
        {
            ConsumeEvent();
        }

        if(_currentEvent->HasCategory(EventCategoryInput) && (Application::Get().GetWindow().ImGuiHasFocus() || _blockAppInputs)) //ImGui should block any app input
        {
            ConsumeEvent();
        }

        // Flushing events will always return false and bypass any consuming
        if(_currentEvent->IsConsumed())
        {
            return;
        }

        if(_appEventDelegate) _appEventDelegate(*_currentEvent);
    }

    void EventHandler::BindEditorEvent(const Delegate<void(Event&)>& delegate)
    {
        _editorEventDelegate += delegate;
    }

    void EventHandler::BindAppEvent(const Delegate<void(Event&)>& delegate)
    {
        _appEventDelegate += delegate;
    }

    void EventHandler::SetBlockAllAppEvents(const bool enable)
    {
        _blockAllAppEvents = enable;
        if(enable)
        {
            InputFlushEvent flushEvent;
            if(_appEventDelegate) _appEventDelegate(flushEvent);
        }
    }

    void EventHandler::SetBlockAppInputs(const bool enable)
    {
        _blockAppInputs = enable;
        if(enable)
        {
            InputFlushEvent flushEvent;
            if(_appEventDelegate) _appEventDelegate(flushEvent);
        }
    }
}
