#include "nppch.h"
#include "InputAction.h"

namespace Engine
{
    bool InputAction::PopulateInput(const float input)
    {
        if (_inputValue.GetIntValue() == (int)input) return false;

        _inputValue = input;

        if (_inputDelegate) _inputDelegate(_inputValue);

        return false;
    }

    void InputAction::FlushAction()
    {
        if (_inputValue.GetIntValue() == 0) return;

        _inputValue = 0;

        if (_inputDelegate) _inputDelegate(_inputValue);
    }

    bool InputTrigger::PopulateInput(const float input)
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

    bool InputAxis::PopulateInput(const float input)
    {
        float axisInput = (input / 32767);

        if (_inputValue.GetAxis() == axisInput) return false;

        _inputValue = axisInput;

        if (_inputDelegate) _inputDelegate(_inputValue);

        return false;
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
