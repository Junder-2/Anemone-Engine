#pragma once
#include "ANE/Subsystem/Subsystem.h"
#include "InputAction.h"
#include "InputHandler.h"

namespace Engine
{
    //Todo: use a abstract service locator
    class InputSystem : public Subsystem
    {
    public:
        InputSystem();
        ~InputSystem() = default;

        void OnEvent(Event& e);

        template <class TClass>
        void BindInput(InputDeviceType deviceType, int bindingId, DelegateMember<TClass, void(InputValue)> delegateMember);

        template <class TClass>
        void BindKeyboardInput(KeyCodes bindingId, DelegateMember<TClass, void(InputValue)> delegateMember);

        template <class TClass>
        void BindMouseMove(DelegateMember<TClass, void(MouseMoveValue)> delegateMember);

        template <class TClass>
        void BindMouseButton(MouseButton buttonIndex, DelegateMember<TClass, void(InputValue)> delegateMember);

        template <class TClass>
        void BindAnyMouseButton(DelegateMember<TClass, void(MouseButtonValues)> delegateMember);

        template <class TClass>
        void BindMouseScroll(DelegateMember<TClass, void(float, float)> delegateMember);

        TriggerState GetKeyTriggerState(const int keyCode) const { return _inputHandler->GetKeyTriggerState(keyCode); }
        // todo: just using for debug now. probably dirty to use array
        std::array<InputValue, 4> GetCurrentTriggeredKeys() const { return _inputHandler->GetCurrentTriggeredKeys(); }

        glm::vec2 GetMouseDelta() const { return _inputHandler->GetMouseInputData().GetMoveValue().GetMouseDelta(); }
        glm::vec2 GetMousePos() const { return _inputHandler->GetMouseInputData().GetMoveValue().GetMousePos(); }
        glm::vec2 GetMouseScroll() const { return _inputHandler->GetMouseInputData().GetScrollValue(); }
        TriggerState GetMouseTriggerState(const int index) const { return _inputHandler-> GetMouseInputData().GetButtonValue().GetTriggerState(index); }
        MouseButtonValues GetMouseButtonValues() const { return _inputHandler->GetMouseInputData().GetButtonValue(); }

    private:
        InputHandler* _inputHandler;

        std::unordered_map<IntPair, MulticastDelegate<void(InputValue)>> _actionMappingDelegates;
        MulticastDelegate<void(MouseMoveValue)> _mouseMoveDelegate;
        MulticastDelegate<void(MouseButtonValues)> _mouseButtonValueDelegate;
        MulticastDelegate<void(float, float)> _mouseScrollDelegate;
    };

    template <class TClass>
    void InputSystem::BindInput(const InputDeviceType deviceType, const int bindingId, DelegateMember<TClass, void(InputValue)> delegateMember)
    {
        const IntPair bindingPair(deviceType, bindingId);

        switch(deviceType)
        {
            case InputDeviceKeyboard:
                _inputHandler->RegisterKeyboardKey(bindingId);
            break;
        }

        _actionMappingDelegates[bindingPair] += delegateMember;
    }

    template <class TClass>
    void InputSystem::BindKeyboardInput(KeyCodes bindingId, DelegateMember<TClass, void(InputValue)> delegateMember)
    {
        BindInput(InputDeviceKeyboard, bindingId, delegateMember);
    }

    template <class TClass>
    void InputSystem::BindMouseMove(DelegateMember<TClass, void(MouseMoveValue)> delegateMember)
    {
        _mouseMoveDelegate += delegateMember;
    }

    template <class TClass>
    void InputSystem::BindMouseButton(MouseButton buttonIndex, DelegateMember<TClass, void(InputValue)> delegateMember)
    {
        BindInput(InputDeviceMouse, buttonIndex, delegateMember);
    }

    template <class TClass>
    void InputSystem::BindAnyMouseButton(DelegateMember<TClass, void(MouseButtonValues)> delegateMember)
    {
        _mouseButtonValueDelegate += delegateMember;
    }

    template <class TClass>
    void InputSystem::BindMouseScroll(DelegateMember<TClass, void(float, float)> delegateMember)
    {
        _mouseScrollDelegate += delegateMember;
    }
}
