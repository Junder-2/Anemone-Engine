#pragma once
#include "Event.h"

namespace Engine
{
    class EventHandler
    {
    public:
        static void PushEvent(Event* newEvent)
        {
            _currentEvent = newEvent;
        }

        static void ClearEvent()
        {
            _currentEvent = nullptr;
        }

        static void ConsumeEvent()
        {
            if(_currentEvent == nullptr)
            {
                NP_ENGINE_LOG_WARN("Cannot consume event outside of event handling");
                return;
            }
            _currentEvent->Consume();
        }

    private:
        inline static Event* _currentEvent {};
    };
}
