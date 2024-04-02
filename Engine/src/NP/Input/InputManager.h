#pragma once

#include <queue> // todo: we can probably precomile this

#include "InputAction.h"
#include "MouseInputAction.h" // todo: include this in the input action?
#include "../Delegate/Delegate.h" // todo: add to include directories in premake so we don't have to format it like this

namespace Engine
{
    class InputManager
    {
    public:
        static std::unique_ptr<InputManager> Create();

        InputManager();
        ~InputManager();

        void RegisterKeyboardTrigger(int keyCode);
        void RegisterKeyboardTwoKeyAxis(int negativeKeyCode, int positiveKeyCode);

        template <class TClass>
        void BindKeyboardTrigger(int keyCode, DelegateMember<TClass, void(InputValue)> delegateMember);
        template <class TClass>
        void BindKeyboardAxis(int negativeKeyCode, int positiveKeyCode, DelegateMember<TClass, void(InputValue)> delegateMember);
        template <class TClass>
        void BindMouseMoveAction(DelegateMember<TClass, void(MouseMoveValue)> delegateMember);
        template <class TClass>
        void BindMouseButtonAction(DelegateMember<TClass, void(MouseButtonValue)> delegateMember);

        void OnUpdate();
        void FlushInputs();
        void PopulateKeyStates(const Uint8* newKeyStates);
        void ProcessKey(int keyCode, bool press);
        void ProcessMouseMovement(float xPos, float yPos, float deltaTime);
        void ProcessMouseButton(int index, bool press, bool isDoubleClick = false);

        TriggerState GetKeyTriggerState(int keyCode);

        glm::vec2 GetMouseDelta() const { return _mouseInputAction.GetMoveValue().GetMouseDelta(); }
        glm::vec2 GetMousePos() const { return _mouseInputAction.GetMoveValue().GetMousePos(); }
        TriggerState GetMouseTriggerState(const int index) const { return _mouseInputAction.GetButtonValue().GetTriggerState(index); }

    private:
        const Uint8* _currentKeyStates{};
        std::unordered_map<int, InputAction*> _keyboardInputActions {};
        std::unordered_map<IntPair, TwoBindingInput*> _keyboardTwoBindings {};
        std::queue<int> _dirtyKeys {};
        MouseInputAction _mouseInputAction {};
        bool _dirtyMouse = false;
    };

    template <class TClass>
    void InputManager::BindKeyboardTrigger(const int keyCode, DelegateMember<TClass, void(InputValue)> delegateMember)
    {
        if(!_keyboardInputActions.contains(keyCode))
        {
            RegisterKeyboardTrigger(keyCode);
        }

        _keyboardInputActions[keyCode]->BindAction(delegateMember);
    }

    template <class TClass>
    void InputManager::BindKeyboardAxis(const int negativeKeyCode, const int positiveKeyCode, DelegateMember<TClass, void(InputValue)> delegateMember)
    {
        const IntPair twoKeys(negativeKeyCode, positiveKeyCode);

        if(!_keyboardTwoBindings.contains(twoKeys))
        {
            RegisterKeyboardTwoKeyAxis(negativeKeyCode, positiveKeyCode);
        }

        TwoBindingInput* twoKeyInputBinding = _keyboardTwoBindings[twoKeys];

        if(!_keyboardInputActions.contains(negativeKeyCode))
        {
            RegisterKeyboardTrigger(negativeKeyCode);
        }
        if(!_keyboardInputActions.contains(positiveKeyCode))
        {
            RegisterKeyboardTrigger(positiveKeyCode);
        }

        _keyboardInputActions[negativeKeyCode]->BindAction(MakeDelegate(twoKeyInputBinding, &TwoBindingInput::OnBoundInput));
        _keyboardInputActions[positiveKeyCode]->BindAction(MakeDelegate(twoKeyInputBinding, &TwoBindingInput::OnBoundInput));

        twoKeyInputBinding->BindAction(delegateMember);
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
