#pragma once
#include "Event.h"
#include "ANE/Delegate/Delegate.h"

namespace Engine
{
    //TODO: might turn this into a subsystem and not a static class

    /**
    * Global class to handle current event
    */
    class EventHandler
    {
    public:
        /**
        * Sends out events to editor subscribers in the editor stage
        */
        static void DispatchEditorEvents();

        /**
        * Sends out events to app subscribers in the app stage
        */
        static void DispatchAppEvents();

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
        * Consumes current event (certain events will be marked as flush and cannot be consumed)
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

        /**
        * Subscribes to the OnEvent call at the editor stage
        * @param delegateMember method with void(Event&)
        */
        template <class TClass>
        static void BindEditorEvent(DelegateMember<TClass, void(Event&)> delegateMember)
        {
            _editorEventDelegate += delegateMember;
        }

        /**
        * Subscribes to the OnEvent call at the app stage
        * @param delegateMember method with void(Event&)
        */
        template <class TClass>
        static void BindAppEvent(DelegateMember<TClass, void(Event&)> delegateMember)
        {
            _appEventDelegate += delegateMember;
        }

        /**
        * Blocks all events from passing into the app stage (certain events will be marked as flush and will still pass)
        */
        static void SetBlockAllAppEvents(const bool enable)
        {
            _blockAllAppEvents = enable;
        }
        static bool IsBlockingAllAppEvents() { return _blockAllAppEvents; }

        /**
        * Blocks all input events from passing into the app stage (certain events will be marked as flush and will still pass)
        */
        static void SetBlockAppInputs(const bool enable)
        {
            _blockAppInputs = enable;
        }
        static bool IsBlockingAppInputs() { return _blockAppInputs; }

    private:
        inline static bool _blockAllAppEvents = false;
        inline static bool _blockAppInputs = false;
        inline static Event* _currentEvent {};

        inline static MulticastDelegate<void(Event&)> _editorEventDelegate;
        inline static MulticastDelegate<void(Event&)> _appEventDelegate;
    };
}
