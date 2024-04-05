#pragma once
#include "Event.h"

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
                ANE_ENGINE_LOG_WARN("Cannot consume event outside of event handling");
                return;
            }
            _currentEvent->Consume();
        }

    private:
        inline static Event* _currentEvent {};
    };
}
