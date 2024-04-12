#pragma once
#include "Event.h"
#include "ANE/Delegate/Delegate.h"

namespace Engine
{
    /**
    * Global class to handle current event
    */
    class EventHandler
    {
    public:
        /**
        * Sets the new current event
        */
        static void PushEvent(Event* newEvent)
        {
            _currentEvent = newEvent;
        }

        /**
        * Clears current event
        */
        static void ClearEvent()
        {
            _currentEvent = nullptr;
        }

        /**
        * Consumes current event
        */
        static void ConsumeEvent()
        {
            if(_currentEvent == nullptr)
            {
                ANE_ELOG_WARN("Cannot consume event outside of event handling");
                return;
            }
            _currentEvent->Consume();
        }

        //TODO: TEMPORARY, bindings and execution should probably be moved to something thats not a static class
        template <class TClass>
        static void BindEditorEvent(DelegateMember<TClass, void(Event&)> delegateMember)
        {
            _editorEventDelegate += delegateMember;
        }

        template <class TClass>
        static void BindAppEvent(DelegateMember<TClass, void(Event&)> delegateMember)
        {
            _appEventDelegate += delegateMember;
        }

        static void SetBlockAllAppEvents(const bool state)
        {
            _blockAllAppEvents = state;
        }

        static bool IsBlockingAllAppEvents() { return _blockAllAppEvents; }

        static void SetBlockAppInputs(const bool state)
        {
            _blockAppInputs = state;
        }

        static bool IsBlockingAppInputs() { return _blockAppInputs; }

        static void DispatchEditorEvents();

        static void DispatchAppEvents();

    private:
        inline static bool _blockAllAppEvents = false;
        inline static bool _blockAppInputs = false;
        inline static Event* _currentEvent {};

        inline static MulticastDelegate<void(Event&)> _editorEventDelegate;
        inline static MulticastDelegate<void(Event&)> _appEventDelegate;
    };
}
