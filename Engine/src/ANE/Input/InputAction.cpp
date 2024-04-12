#include "anepch.h"
#include "InputAction.h"

namespace Engine
{
    bool InputAction::PopulateInput(const float input, bool* needProcessing)
    {
        if (_inputValue.GetIntValue() == (int)input) return false;

        _inputValue = input;

        return true;
    }

    bool InputAction::FlushAction()
    {
        if (_inputValue.GetIntValue() == 0) return false;

        _inputValue = 0;

        return true;
    }

    bool InputTrigger::PopulateInput(const float input, bool* needProcessing)
    {
        const bool shouldForward = InputAction::PopulateInput(input, needProcessing);

        *needProcessing = true;

        return shouldForward;
    }

    bool InputTrigger::ProcessAction(bool* needProcessing)
    {
        bool shouldForward = false;

        switch (_inputValue.GetTriggerState())
        {
            case TriggerStarted:
                _inputValue = TriggerHolding;
                *needProcessing = true;
                shouldForward = true;
            break;
            case TriggerHolding:
                *needProcessing = true;
                shouldForward = true;
            break;
            case TriggerStopped:
                _inputValue = TriggerNone;
            break;
        }

        return shouldForward;
    }

    bool InputTrigger::FlushAction()
    {
        if (_inputValue.GetIntValue() == 0) return false;

        _inputValue = TriggerStopped;

        return true;
    }

    bool InputAxis::PopulateInput(const float input, bool* needProcessing)
    {
        //should convert in window later
        float axisInput = (input / 32767);

        if (_inputValue.GetAxis() == axisInput) return false;

        _inputValue = axisInput;

        return true;
    }

    void TwoBindingInput::OnBoundInput(InputValue inputValue)
    {
        const int bindingId = inputValue.GetBindingId();

        if(bindingId == _negativeBindingId)
        {
            _inputValue = inputValue.GetTriggerState() == TriggerStarted ? -1.0f : .0f;
        }
        else if(bindingId == _positiveBindingId)
        {
            _inputValue = inputValue.GetTriggerState() == TriggerStarted ? 1.0f : .0f;
        }
        else
        {
            ANE_ELOG_WARN("TwoKeyInputBinding is incorrectly set up");
            return;
        }
    }
}
