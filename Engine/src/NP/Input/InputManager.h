#pragma once

#include <queue>

#include "InputAction.h"
#include "MouseInputAction.h"
#include "../Delegate/Delegate.h"

namespace Engine
{
    class InputManager
    {
    public:
        static std::unique_ptr<InputManager> Create();

        InputManager();
        ~InputManager();

        void RegisterKeyboardTrigger(int keyCode);

        template <class TClass>
        void BindKeyboardTrigger(int keyCode, DelegateMember<TClass, void(InputValue)> delegateMember);
        template <class TClass>
        void BindMouseMoveAction(DelegateMember<TClass, void(MouseMoveValue)> delegateMember);
        template <class TClass>
        void BindMouseButtonAction(DelegateMember<TClass, void(MouseButtonValue)> delegateMember);

        void OnUpdate();
        void ProcessKey(int keyCode, bool press);
        void ProcessMouseMovement(float xPos, float yPos, float deltaTime);
        void ProcessMouseButton(int index, bool press);

        TriggerState GetKeyTriggerState(int keyCode);

        glm::vec2 GetMouseDelta() const { return _mouseInputAction.GetMoveValue().GetMouseDelta(); }
        glm::vec2 GetMousePos() const { return _mouseInputAction.GetMoveValue().GetMousePos(); }
        TriggerState GetMouseTriggerState(const int index) const { return _mouseInputAction.GetButtonValue().GetTriggerState(index); }


    private:
        std::map<int, InputAction*> _keyboardInputActions {};
        std::queue<int> _dirtyKeys {};
        MouseInputAction _mouseInputAction {};
        bool _dirtyMouse = false;
    };

    template <class TClass>
    void InputManager::BindKeyboardTrigger(const int keyCode, DelegateMember<TClass, void(InputValue)> delegateMember)
    {
        if(_keyboardInputActions.contains(keyCode))
        {
            _keyboardInputActions[keyCode]->BindAction(delegateMember);
            return;
        }

        InputTrigger* newInputAction = new InputTrigger();

        newInputAction->BindAction(delegateMember);

        _keyboardInputActions.insert_or_assign(keyCode, newInputAction);
    }

    template <class TClass>
    void InputManager::BindMouseMoveAction(DelegateMember<TClass, void(MouseMoveValue)> delegateMember)
    {
        _mouseInputAction.BindMoveAction(delegateMember);
    }

    template <class TClass>
    void InputManager::BindMouseButtonAction(DelegateMember<TClass, void(MouseButtonValue)> delegateMember)
    {
        _mouseInputAction.BindButtonAction(delegateMember);
    }
}
