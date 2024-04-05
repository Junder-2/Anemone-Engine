#pragma once

#include "InputTypes.h"

namespace Engine
{
    /**
    * Container for a generic input value
    */
    struct InputValue
    {
    public:
        InputValue(const int bindingId = 0) : _bindingId(bindingId) { }
        InputValue(const InputValueType type, const InputSourceType sourceType, const int bindingId = 0) : _valueType(type), _sourceType(sourceType), _bindingId(bindingId) { }

        /**
        * Returns the value type
        */
        InputValueType GetValueType() const
        {
            return _valueType;
        }

        /**
        * Returns the input source
        */
        InputValueType GetSourceType() const
        {
            return _valueType;
        }

        /**
        * Returns a input value in integer form
        */
        int GetIntValue() const
        {
            return static_cast<int>(_value);
        }

        /**
        * Returns a input value in TriggerState form
        */
        TriggerState GetTriggerState() const
        {
            return static_cast<TriggerState>(GetIntValue());
        }

        /**
        * Returns a input value in axis/float form
        */
        float GetAxis() const
        {
            return _value;
        }

        /**
        * Returns id of the input binding
        */
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

    /**
    * Base class for a input action, contains logic to process and interpret inputs
    */
    class InputAction
    {
    public:
        InputAction(const InputValueType type, const InputSourceType sourceType, const int bindingId = 0) : _inputValue(InputValue(type, sourceType, bindingId)) {}
        virtual ~InputAction() = default;

        /**
        * Populates new input
        * @param needProcessing if this action needs processing next frame
        * @returns stored data changed
        */
        virtual bool PopulateInput(float input, bool* needProcessing);
        /**
        * Processes the input data
        * @param needProcessing if this action needs processing next frame
        * @returns stored data changed
        */
        virtual bool ProcessAction(bool* needProcessing) = 0;
        /**
        * Clears the input data
        * @returns stored data changed
        */
        virtual bool FlushAction();

        InputValue GetInputValue() const { return _inputValue; }

    protected:
        InputValue _inputValue;
    };

    /**
    * Input action that interprets inputs as a TriggerState
    */
    class InputTrigger final : public InputAction
    {
    public:
        InputTrigger(const InputSourceType sourceType, const int bindingId = 0) : InputAction(InputTypeTrigger, sourceType, bindingId) {}

        /**
        * Populates new input
        * @param needProcessing if this action needs processing next frame
        * @returns stored data changed
        */
        bool PopulateInput(float input, bool* needProcessing) override;
        /**
        * Processes action by transitions started to hold and stopped to none
        * @param needProcessing if this action needs processing next frame
        * @returns stored data changed
        */
        bool ProcessAction(bool* needProcessing) override;
    };

    /**
    * Input action that interprets inputs as a axis/float
    */
    class InputAxis : public InputAction
    {
    public:
        InputAxis(const InputSourceType sourceType, const int bindingId = 0) : InputAction(InputTypeAxis, sourceType, bindingId) {}

        /**
        * Populates new input
        * @param needProcessing if this action needs processing next frame
        * @returns stored data changed
        */
        bool PopulateInput(float input, bool* needProcessing) override;
    };

    //WIP
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