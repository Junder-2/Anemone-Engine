#pragma once

#include <entt.hpp>
#include <queue> // todo: we can probably precomile this

#include "InputAction.h"
#include "NP/Events/Event.h"
#include "NP/Delegate/Delegate.h"

namespace Engine
{
    /**
    * Class that stores and processes inputs
    */
    class InputManager
    {
    public:
        static std::unique_ptr<InputManager> Create();

        InputManager();
        ~InputManager();

        void OnUpdate();
        SinglecastDelegate<void(Event&)> EventDelegate;

        /**
        * Registers a keyboard key to be handled and processed
        * @param keyCode id from KeyCodes
        */
        void RegisterKeyboardTrigger(int keyCode);
        /**
        * Registers a keyboard input to be handled and processed
        * @param negativeKeyCode id from KeyCodes
        * @param positiveKeyCode id from KeyCodes
        */
        void RegisterKeyboardTwoKeyAxis(int negativeKeyCode, int positiveKeyCode);

        void PopulateKeyStates(const Uint8* newKeyStates);
        void ProcessKey(int keyCode, bool press);
        void ProcessMouseMovement(float xPos, float yPos, float deltaTime);
        void ProcessMouseButton(int index, bool press, bool isDoubleClick = false);
        void ProcessMouseScroll(float xDelta, float yDelta);
        /**
        * Clears current inputs
        */
        void FlushInputs();

        /**
        * Returns the trigger state of a key. If the requested key isn't registered returns only pressed or not pressed
        */
        TriggerState GetKeyTriggerState(int keyCode);
        /**
        * Returns four currently triggered keys
        */
        // todo: just using for debug now. probably dirty to use array
        std::array<InputValue, 4> GetCurrentTriggeredKeys();

        glm::vec2 GetMouseDelta() const { return _mouseInputAction.GetMoveValue().GetMouseDelta(); }
        glm::vec2 GetMousePos() const { return _mouseInputAction.GetMoveValue().GetMousePos(); }
        TriggerState GetMouseTriggerState(const int index) const { return _mouseInputAction.GetButtonValue().GetTriggerState(index); }
        MouseButtonValues GetMouseButtonValues() const { return _mouseInputAction.GetButtonValue(); }

    private:
        void DispatchEvent(Event& e);

    private:
        const Uint8* _currentKeyStates{};
        entt::dense_map<int, InputAction*> _keyboardInputActions {};
        std::unordered_map<IntPair, TwoBindingInput*> _keyboardTwoBindings {};
        std::queue<int> _dirtyKeys {};
        MouseInputAction _mouseInputAction {};
        bool _dirtyMouse = false;
    };

    // template <class TClass>
    // void InputManager::BindKeyboardAxis(const int negativeKeyCode, const int positiveKeyCode, DelegateMember<TClass, void(InputValue)> delegateMember)
    // {
    //     const IntPair twoKeys(negativeKeyCode, positiveKeyCode);
    //
    //     if(!_keyboardTwoBindings.contains(twoKeys))
    //     {
    //         RegisterKeyboardTwoKeyAxis(negativeKeyCode, positiveKeyCode);
    //     }
    //
    //     TwoBindingInput* twoKeyInputBinding = _keyboardTwoBindings[twoKeys];
    //
    //     if(!_keyboardInputActions.contains(negativeKeyCode))
    //     {
    //         RegisterKeyboardTrigger(negativeKeyCode);
    //     }
    //     if(!_keyboardInputActions.contains(positiveKeyCode))
    //     {
    //         RegisterKeyboardTrigger(positiveKeyCode);
    //     }
    //
    //     _keyboardInputActions[negativeKeyCode]->BindAction(MakeDelegate(twoKeyInputBinding, &TwoBindingInput::OnBoundInput));
    //     _keyboardInputActions[positiveKeyCode]->BindAction(MakeDelegate(twoKeyInputBinding, &TwoBindingInput::OnBoundInput));
    //
    //     twoKeyInputBinding->BindAction(delegateMember);
    // }
}
