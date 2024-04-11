#pragma once
#include "ANE/Subsystem/Subsystem.h"
#include "InputAction.h"
#include "InputHandler.h"

namespace Engine
{
    class InputSystem : public Subsystem
    {
    public:
        InputSystem();
        ~InputSystem() = default;

        void OnEvent(Event& e);

        template <class TClass>
        void BindInput(BindingPair bindingPair, DelegateMember<TClass, void(InputValue)> delegateMember);

        template <class TClass>
        void BindAxisInput(BindingPair negativeBindingPair, BindingPair positiveBindingPair, DelegateMember<TClass, void(InputValue)> delegateMember);

        template <class TClass>
        void BindKeyboardInput(KeyCodes bindingId, DelegateMember<TClass, void(InputValue)> delegateMember);

        template <class TClass>
        void BindKeyboardAxisInput(KeyCodes negativeBindingId, KeyCodes positiveBindingId, DelegateMember<TClass, void(InputValue)> delegateMember);

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

        Vector2 GetMouseDelta() const { return _inputHandler->GetMouseInputData().GetMoveValue().GetMouseDelta(); }
        Vector2 GetMousePos() const { return _inputHandler->GetMouseInputData().GetMoveValue().GetMousePos(); }
        Vector2 GetMouseScroll() const { return _inputHandler->GetMouseInputData().GetScrollValue(); }
        TriggerState GetMouseTriggerState(const int index) const { return _inputHandler-> GetMouseInputData().GetButtonValue().GetTriggerState(index); }
        MouseButtonValues GetMouseButtonValues() const { return _inputHandler->GetMouseInputData().GetButtonValue(); }

    private:
        bool IsValidAxisBindings(BindingPair negativeBindingPair, BindingPair positiveBindingPair);

    private:
        InputHandler* _inputHandler;

        std::unordered_map<BindingPair, MulticastDelegate<void(InputValue)>> _actionMappingDelegates {};
        std::unordered_map<BindingPair, AxisBinding> _bindingToAxisBinding {};
        std::unordered_map<int, MulticastDelegate<void(InputValue)>> _axisActionMappingDelegates {};
        MulticastDelegate<void(MouseMoveValue)> _mouseMoveDelegate {};
        MulticastDelegate<void(MouseButtonValues)> _mouseButtonValueDelegate {};
        MulticastDelegate<void(float, float)> _mouseScrollDelegate {};
    };

    template <class TClass>
    void InputSystem::BindInput(const BindingPair bindingPair, DelegateMember<TClass, void(InputValue)> delegateMember)
    {
        switch(bindingPair.DeviceType)
        {
            case InputDeviceKeyboard:
                _inputHandler->RegisterKeyboardKey(bindingPair.BindingId);
            break;
        }

        _actionMappingDelegates[bindingPair] += delegateMember;
    }

    template <class TClass>
    void InputSystem::BindKeyboardInput(const KeyCodes bindingId, DelegateMember<TClass, void(InputValue)> delegateMember)
    {
        BindInput(BindingPair(InputDeviceKeyboard, bindingId), delegateMember);
    }

    template <class TClass>
    void InputSystem::BindMouseButton(const MouseButton buttonIndex, DelegateMember<TClass, void(InputValue)> delegateMember)
    {
        BindInput(BindingPair(InputDeviceMouse, buttonIndex), delegateMember);
    }

    template <class TClass>
    void InputSystem::BindMouseMove(DelegateMember<TClass, void(MouseMoveValue)> delegateMember)
    {
        _mouseMoveDelegate += delegateMember;
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

    template <class TClass>
    void InputSystem::BindAxisInput(const BindingPair negativeBindingPair, const BindingPair positiveBindingPair, DelegateMember<TClass, void(InputValue)> delegateMember)
    {
        if (IsValidAxisBindings(negativeBindingPair, positiveBindingPair)) return;

        switch(positiveBindingPair.DeviceType)
        {
            case InputDeviceKeyboard:
                _inputHandler->RegisterKeyboardKey(positiveBindingPair.BindingId);
            break;
        }

        switch(negativeBindingPair.DeviceType)
        {
            case InputDeviceKeyboard:
                _inputHandler->RegisterKeyboardKey(negativeBindingPair.BindingId);
            break;
        }

        int size = _bindingToAxisBinding.size();
        if(size != 0 && size % 2 != 0)
        {
            ANE_ELOG_WARN("axis binding size is not multiple of 2: {}", size);
            size++;
        }
        const int id = size / 2;

        _bindingToAxisBinding.insert_or_assign(positiveBindingPair, AxisBinding(id, true));
        _bindingToAxisBinding.insert_or_assign(negativeBindingPair, AxisBinding(id, false));

        _axisActionMappingDelegates[id] += delegateMember;
    }

    template <class TClass>
    void InputSystem::BindKeyboardAxisInput(const KeyCodes negativeBindingId, const KeyCodes positiveBindingId, DelegateMember<TClass, void(InputValue)> delegateMember)
    {
        BindAxisInput(BindingPair(InputDeviceKeyboard, negativeBindingId), BindingPair(InputDeviceKeyboard, positiveBindingId), delegateMember);
    }
}
