#pragma once

#include "InputTypes.h"
#include "../Delegate/Delegate.h"

namespace Engine
{
    struct InputValue
    {
    public:
        int GetRawValue() const
        {
            return _value;
        }

        TriggerState GetTriggerState()
        {
            return static_cast<TriggerState>(_value);
        }

        float GetAxis() const
        {
            return static_cast<float>(_value) / 32767.f;
        }

        InputValue& operator=(const int rhs)
        {
            _value = rhs;
            return *this;
        }

    protected:
        int _value = 0;
    };

    class InputAction
    {
    public:
        InputAction() = default;
        virtual ~InputAction() = default;

        template <class TClass>
        void BindAction(DelegateMember<TClass, void(InputValue)> delegateMember);
        virtual void UpdateAction() = 0;
        void PopulateInput(int input);

        InputValue GetInputValue() const { return _inputValue; }

    protected:
        InputValue _inputValue;
        MulticastDelegate<void(InputValue)> _inputDelegate;
    };

    class InputTrigger : public InputAction
    {
    public:
        InputTrigger() = default;
        ~InputTrigger() override = default;

        void UpdateAction() override;
    };

    template <class TClass>
    void InputAction::BindAction(DelegateMember<TClass, void(InputValue)> delegateMember)
    {
        _inputDelegate += delegateMember;
    }
}
