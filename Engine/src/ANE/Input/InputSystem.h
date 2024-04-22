#pragma once
#include "ANE/Subsystem/SubSystem.h"
#include "InputAction.h"
#include "InputHandler.h"

namespace Engine
{
    class InputSystem : public SubSystem
    {
    public:
        InputSystem();
        ~InputSystem() override = default;

        /**
        * Binds an input to a method (Unregistered inputs will be auto registered)
        * @param bindingPair holds device type and binding id
        * @param delegateMember method with void(InputValue)
        */
        template <class TClass>
        void BindInput(BindingPair bindingPair, DelegateMember<TClass, void(InputValue)> delegateMember);

        /**
        * Binds two inputs to a method (Unregistered inputs will be auto registered)
        * @param negativeBindingPair holds device type and binding id, decides negative sign
        * @param positiveBindingPair holds device type and binding id, decides positive sign
        * @param delegateMember method with void(InputValue)
        */
        template <class TClass>
        void BindAxisInput(BindingPair negativeBindingPair, BindingPair positiveBindingPair, DelegateMember<TClass, void(InputValue)> delegateMember);

        /**
        * Binds a keyboard input to a method (Unregistered inputs will be auto registered)
        * @param keyCode the keycode of the input
        * @param delegateMember method with void(InputValue)
        */
        template <class TClass>
        void BindKeyboardInput(KeyCodes keyCode, DelegateMember<TClass, void(InputValue)> delegateMember);

        /**
        * Binds a keyboard input to a method (Unregistered inputs will be auto registered)
        * @param negativeKeyCode the keycode of the input, decides negative sign
        * @param positiveKeyCode the keycode of the input, decides positive sign
        * @param delegateMember method with void(InputValue)
        */
        template <class TClass>
        void BindKeyboardAxisInput(KeyCodes negativeKeyCode, KeyCodes positiveKeyCode, DelegateMember<TClass, void(InputValue)> delegateMember);

        /**
        * Binds mouse movement input to a method
        * @param delegateMember method with void(MouseMoveValue)
        */
        template <class TClass>
        void BindMouseMove(DelegateMember<TClass, void(MouseMoveValue)> delegateMember);

        /**
        * Binds a mouse button input to a method
        * @param buttonIndex MouseButton
        * @param delegateMember method with void(InputValue)
        */
        template <class TClass>
        void BindMouseButton(MouseButton buttonIndex, DelegateMember<TClass, void(InputValue)> delegateMember);

        /**
        * Binds any mouse button input to a method
        * @param delegateMember method with void(MouseButtonValues)
        */
        template <class TClass>
        void BindAnyMouseButton(DelegateMember<TClass, void(MouseButtonValues)> delegateMember);

        /**
        * Binds a mouse scroll to a method
        * @param delegateMember method with void(float, float)
        */
        template <class TClass>
        void BindMouseScroll(DelegateMember<TClass, void(Vector2)> delegateMember);

        /**
        * Returns the trigger state of a key. If the requested key isn't registered returns only pressed or not pressed
        */
        TriggerState GetKeyTriggerState(const int keyCode) const { return _inputHandler->GetKeyTriggerState(keyCode); }
        // todo: just using for debug now. probably dirty to use array
        std::array<InputValue, 4> GetCurrentTriggeredKeys() const { return _inputHandler->GetCurrentTriggeredKeys(); }

        /**
        * Returns current mouse pos
        */
        Vector2 GetAbsoluteMousePos() const { return _inputHandler->GetMouseInputData().GetMoveValue().GetAbsoluteMousePos(); }
        /**
        * Returns current mouse pos
        */
        Vector2 GetMousePos() const { return _inputHandler->GetMouseInputData().GetMoveValue().GetMousePos(); }
        /**
        * Returns current mouse delta
        */
        Vector2 GetMouseDelta() const { return _inputHandler->GetMouseInputData().GetMoveValue().GetMouseDelta(); }
        /**
        * Returns current mouse scroll deltas
        */
        Vector2 GetMouseScroll() const { return _inputHandler->GetMouseInputData().GetScrollValue(); }
        /**
        * Returns current TriggerState for a mouse button
        */
        TriggerState GetMouseTriggerState(const int index) const { return _inputHandler-> GetMouseInputData().GetButtonValue().GetTriggerState(index); }
        /**
        * Returns current state of all mouse buttons
        */
        MouseButtonValues GetMouseButtonValues() const { return _inputHandler->GetMouseInputData().GetButtonValue(); }

    protected:
        virtual void OnEvent(Event& e);
        bool IsValidAxisBindings(BindingPair negativeBindingPair, BindingPair positiveBindingPair);

    protected:
        InputHandler* _inputHandler;

        std::unordered_map<BindingPair, MulticastDelegate<void(InputValue)>> _actionMappingDelegates {};
        std::unordered_map<BindingPair, AxisBinding> _bindingToAxisBinding {};
        std::unordered_map<int, MulticastDelegate<void(InputValue)>> _axisActionMappingDelegates {};
        MulticastDelegate<void(MouseMoveValue)> _mouseMoveDelegate {};
        MulticastDelegate<void(MouseButtonValues)> _mouseButtonValueDelegate {};
        MulticastDelegate<void(Vector2)> _mouseScrollDelegate {};
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
    void InputSystem::BindKeyboardInput(const KeyCodes keyCode, DelegateMember<TClass, void(InputValue)> delegateMember)
    {
        BindInput(BindingPair(InputDeviceKeyboard, keyCode), delegateMember);
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
    void InputSystem::BindMouseScroll(DelegateMember<TClass, void(Vector2)> delegateMember)
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
    void InputSystem::BindKeyboardAxisInput(const KeyCodes negativeKeyCode, const KeyCodes positiveKeyCode, DelegateMember<TClass, void(InputValue)> delegateMember)
    {
        BindAxisInput(BindingPair(InputDeviceKeyboard, negativeKeyCode), BindingPair(InputDeviceKeyboard, positiveKeyCode), delegateMember);
    }
}
