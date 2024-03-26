#include "nppch.h"
#include "InputAction.h"

namespace Engine
{
    void InputAction::PopulateInput(const int input)
    {
        if (_inputValue.GetRawValue() == input) return;

        _inputValue = input;

        if (_inputDelegate) _inputDelegate(_inputValue);
    }

    void InputTrigger::UpdateAction()
    {
        switch (_inputValue.GetTriggerState())
        {
            case TriggerStarted:
                _inputValue = TriggerHolding;
            break;
            case TriggerStopped:
                _inputValue = TriggerNone;
        }
    }
}
