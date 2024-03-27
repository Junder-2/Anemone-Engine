#include "nppch.h"
#include "InputAction.h"

namespace Engine
{
    bool InputAction::PopulateInput(const int input)
    {
        if (_inputValue.GetRawValue() == input) return false;

        _inputValue = input;

        if (_inputDelegate) _inputDelegate(_inputValue);

        return false;
    }

    bool InputTrigger::PopulateInput(const int input)
    {
        InputAction::PopulateInput(input);

        return true;
    }

    bool InputTrigger::ProcessAction()
    {
        switch (_inputValue.GetTriggerState())
        {
            case TriggerStarted:
                _inputValue = TriggerHolding;
            break;
            case TriggerStopped:
                _inputValue = TriggerNone;
        }

        return false;
    }
}
