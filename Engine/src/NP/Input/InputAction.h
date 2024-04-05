#pragma once

#include "InputTypes.h"

namespace Engine
{
    struct InputValue
    {
    public:
        InputValue(const int bindingId = 0) : _bindingId(bindingId) { }
        InputValue(const InputValueType type, const InputSourceType sourceType, const int bindingId = 0) : _valueType(type), _sourceType(sourceType), _bindingId(bindingId) { }

        InputValueType GetValueType() const
        {
            return _valueType;
        }

        InputValueType GetSourceType() const
        {
            return _valueType;
        }

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

        int GetBindingId() const
        {
            return _bindingId;
        }

        InputValue& operator=(const float rhs)
        {
            _value = rhs;
            return *this;
        }

    protected:
        float _value = 0;
        InputValueType _valueType = InputTypeBoolean;
        InputSourceType _sourceType = InputSourceKeyboard;
        int _bindingId;
    };

    class InputAction
    {
    public:
        InputAction(const InputValueType type, const InputSourceType sourceType, const int bindingId = 0) : _inputValue(InputValue(type, sourceType, bindingId)) {}
        virtual ~InputAction() = default;

        virtual bool PopulateInput(float input, bool* needProcessing);
        virtual bool ProcessAction(bool* needProcessing) = 0;
        virtual bool FlushAction();

        InputValue GetInputValue() const { return _inputValue; }

    protected:
        InputValue _inputValue;
    };

    class InputTrigger final : public InputAction
    {
    public:
        InputTrigger(const InputSourceType sourceType, const int bindingId = 0) : InputAction(InputTypeTrigger, sourceType, bindingId) {}

        bool PopulateInput(float input, bool* needProcessing) override;
        bool ProcessAction(bool* needProcessing) override;
    };

    class InputAxis : public InputAction
    {
    public:
        InputAxis(const InputSourceType sourceType, const int bindingId = 0) : InputAction(InputTypeAxis, sourceType, bindingId) {}

        bool PopulateInput(float input, bool* needProcessing) override;
    };

    class TwoBindingInput
    {
    public:
        TwoBindingInput(const InputSourceType sourceType, const int negativeBindingId, const int positiveBindingId) //todo: might want to turn the combined binding Id into has so we can decode it later
        : _inputValue(InputTypeAxis, sourceType, negativeBindingId+positiveBindingId), _negativeBindingId(negativeBindingId), _positiveBindingId(positiveBindingId) {}
        ~TwoBindingInput() = default;

        void OnBoundInput(InputValue inputValue);

    private:
        InputValue _inputValue;

        int _negativeBindingId = 0;
        int _positiveBindingId = 0;
    };
}

#include "MouseInputAction.h"
