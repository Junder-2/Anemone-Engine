#pragma once
#include "InputTypes.h"
#include "../Delegate/Delegate.h"
#include "glm/vec2.hpp"

namespace Engine
{
    struct MouseMoveValue
    {
    public:
        glm::vec2 GetMousePos() const { return _mousePosition; }
        glm::vec2 GetMouseDelta() const { return _mouseDelta; }

        void SetMousePos(const float x, const float y)
        {
            _mousePosition.x = x;
            _mousePosition.y = y;
        }

        void SetMouseDelta(const float x, const float y)
        {
            _mouseDelta.x = x;
            _mouseDelta.y = y;
        }

    private:
        glm::vec2 _mouseDelta = {};
        glm::vec2 _mousePosition = {};
    };

    struct MouseButtonValues
    {
        int GetCurrentButtonIndex() const
        {
            return _lastIndex;
        }

        bool GetIsDoubleClick() const
        {
            return _isDoubleClick;
        }

        uint16_t GetRawButtonStates() const
        {
            return _buttonStates;
        }

        TriggerState GetTriggerState() const
        {
            return GetTriggerStateFromMouseButtonState(_buttonStates, _lastIndex);
        }

        TriggerState GetTriggerState(const int index) const
        {
            return GetTriggerStateFromMouseButtonState(_buttonStates, index);
        }

        void SetTriggerState(const int index, const TriggerState newState, const bool isDoubleClick = false)
        {
            _isDoubleClick = isDoubleClick;
            _lastIndex = std::min(index, MOUSE_BUTTON_MAX-1);
            //Clear and add new state to button
            _buttonStates &= ~GetMouseButtonMaskFromMouseButton(index);
            _buttonStates |= GetMouseButtonStateFromMouseButton(newState, index);
        }

    private:
        int _lastIndex = 0;
        bool _isDoubleClick = false;
        uint16_t _buttonStates = 0;
    };

    class MouseInputAction
    {
    public:
        MouseInputAction();
        ~MouseInputAction();

        template <class TClass>
        void BindMoveAction(DelegateMember<TClass, void(MouseMoveValue)> delegateMember);
        template <class TClass>
        void BindButtonAction(DelegateMember<TClass, void(MouseButtonValues)> delegateMember);
        bool PopulateMoveInput(bool* needProcessing, float x, float y, float deltaTime);
        bool PopulateButtonInput(bool* needProcessing, int buttonIndex, TriggerState newState, bool isDoubleClick = false);
        bool PopulateScrollInput(bool* needProcessing, float x, float y);
        void ProcessAction();
        void FlushAction();

        MouseMoveValue GetMoveValue() const { return _moveValue; }
        MouseButtonValues GetButtonValue() const { return _buttonValue; }
        glm::vec2 GetScrollValue() const { return _scrollWheelValue; }

    protected:
        MouseMoveValue _moveValue {};
        MouseButtonValues _buttonValue {};
        glm::vec2 _scrollWheelValue {};
    };
}
