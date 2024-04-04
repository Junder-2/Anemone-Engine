#include "nppch.h"
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
        const bool changed = InputAction::PopulateInput(input, needProcessing);

        *needProcessing = true;

        return changed;
    }

    bool InputTrigger::ProcessAction(bool* needProcessing)
    {
        switch (_inputValue.GetTriggerState())
        {
            case TriggerStarted:
                _inputValue = TriggerHolding;
            break;
            case TriggerStopped:
                _inputValue = TriggerNone;
        }

        *needProcessing = false;
        return true;
    }

    bool InputAxis::PopulateInput(const float input, bool* needProcessing)
    {
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
            NP_ENGINE_LOG_WARN("TwoKeyInputBinding is incorrectly set up");
            return;;
        }

        if(_inputDelegate) _inputDelegate(_inputValue);
    }
}
