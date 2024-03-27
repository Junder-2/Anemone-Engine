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

    struct MouseButtonValue
    {
        static constexpr int indexLength = 3;

        int GetCurrentButtonIndex() const
        {
            return _lastIndex;
        }

        TriggerState GetTriggerState() const
        {
            return _button[_lastIndex];
        }

        TriggerState GetTriggerState(const int index) const
        {
            return _button[std::min(index, indexLength-1)];
        }

        void SetTriggerState(const int index, const TriggerState newState)
        {
            _lastIndex = std::min(index, indexLength-1);
            _button[_lastIndex] = newState;
        }

    private:
        int _lastIndex = 0;
        TriggerState _button[indexLength] = {};
    };

    class MouseInputAction
    {
    public:
        MouseInputAction();
        ~MouseInputAction();

        template <class TClass>
        void BindMoveAction(DelegateMember<TClass, void(MouseMoveValue)> delegateMember);
        template <class TClass>
        void BindButtonAction(DelegateMember<TClass, void(MouseButtonValue)> delegateMember);
        bool PopulateMoveInput(float x, float y, float deltaTime);
        bool PopulateButtonInput(int buttonIndex, TriggerState newState);
        void ProcessAction();

        MouseMoveValue GetMoveValue() const { return _moveValue; }
        MouseButtonValue GetButtonValue() const { return _buttonValue; }

    protected:
        MouseMoveValue _moveValue {};
        MouseButtonValue _buttonValue {};
        MulticastDelegate<void(MouseMoveValue)> _moveInputDelegate;
        MulticastDelegate<void(MouseButtonValue)> _buttonInputDelegate;
    };

    template <class TClass>
    void MouseInputAction::BindMoveAction(DelegateMember<TClass, void(MouseMoveValue)> delegateMember)
    {
        _moveInputDelegate += delegateMember;
    }

    template <class TClass>
    void MouseInputAction::BindButtonAction(DelegateMember<TClass, void(MouseButtonValue)> delegateMember)
    {
        _buttonInputDelegate += delegateMember;
    }
}
