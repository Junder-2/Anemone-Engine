#pragma once
#include "ANE/Delegate/Delegate.h"

namespace Engine
{
    //TODO: might turn this into a subsystem and not a static class
    class Event;

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
        static void ConsumeEvent();

        /**
        * Subscribes to the OnEvent call at the editor stage
        * @param delegate method with void(Event&)
        */
        static void BindEditorEvent(const Delegate<void(Event&)>& delegate);

        /**
        * Subscribes to the OnEvent call at the app stage
        * @param delegate method with void(Event&)
        */
        static void BindAppEvent(const Delegate<void(Event&)>& delegate);

        /**
        * Blocks all events from passing into the app stage (certain events will be marked as flush and will still pass)
        */
        static void SetBlockAllAppEvents(bool enable);
        static bool IsBlockingAllAppEvents() { return _blockAllAppEvents; }

        /**
        * Blocks all input events from passing into the app stage (certain events will be marked as flush and will still pass)
        */
        static void SetBlockAppInputs(bool enable);
        static bool IsBlockingAppInputs() { return _blockAppInputs; }

    private:
        inline static bool _blockAllAppEvents = false;
        inline static bool _blockAppInputs = false;
        inline static Event* _currentEvent {};

        inline static MulticastDelegate<void(Event&)> _editorEventDelegate;
        inline static MulticastDelegate<void(Event&)> _appEventDelegate;
    };
}
