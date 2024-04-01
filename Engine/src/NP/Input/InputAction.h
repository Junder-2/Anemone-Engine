#pragma once

#include "InputTypes.h"
#include "../Delegate/Delegate.h" // todo: add to include directory

namespace Engine
{
    struct InputValue
    {
    public:
        int GetIntValue() const
        {
            return static_cast<int>(_value);
        }

        TriggerState GetTriggerState() const
        {
            return static_cast<TriggerState>(GetIntValue());
        }

        float GetAxis() const
        {
            return _value;
        }

        InputValue& operator=(const float rhs)
        {
            _value = rhs;
            return *this;
        }

    protected:
        float _value = 0;
    };

    class InputAction
    {
    public:
        InputAction() = default;
        virtual ~InputAction() = default;

        template <class TClass>
        void BindAction(DelegateMember<TClass, void(InputValue)> delegateMember);
        virtual bool PopulateInput(float input);
        virtual bool ProcessAction() = 0;
        virtual void FlushAction();

        InputValue GetInputValue() const { return _inputValue; }

    protected:
        InputValue _inputValue;
        MulticastDelegate<void(InputValue)> _inputDelegate;
    };

    class InputTrigger : public InputAction
    {
    public:
        bool PopulateInput(float input) override;
        bool ProcessAction() override;
    };

    class InputAxis : public InputAction
    {
    public:
        bool PopulateInput(float input) override;
    };

    template <class TClass>
    void InputAction::BindAction(DelegateMember<TClass, void(InputValue)> delegateMember)
    {
        _inputDelegate += delegateMember;
    }
}
