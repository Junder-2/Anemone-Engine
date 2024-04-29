#pragma once
#include "InputTypes.h"
#include "ANE/Delegate/Delegate.h"
#include "ANE/Subsystem/SubSystem.h"

namespace Engine
{
    class InputHandler;
    class Event;
    struct Vector2;
    struct MouseButtonValues;
    struct MouseMoveValue;
    struct BindingPair;
    struct InputValue;

    class InputSystem : public SubSystem
    {
    public:
        InputSystem();
        ~InputSystem() override = default;

        /**
        * Binds an input to a method (Unregistered inputs will be auto registered)
        * @param bindingPair holds device type and binding id
        * @param delegate method with void(InputValue)
        */
        void BindInput(BindingPair bindingPair, const Delegate<void(InputValue)>& delegate);

        /**
        * Binds two inputs to a method (Unregistered inputs will be auto registered)
        * @param negativeBindingPair holds device type and binding id, decides negative sign
        * @param positiveBindingPair holds device type and binding id, decides positive sign
        * @param delegate method with void(InputValue)
        */
        void BindAxisInput(BindingPair negativeBindingPair, BindingPair positiveBindingPair, const Delegate<void(InputValue)>& delegate);

        /**
        * Binds a keyboard input to a method (Unregistered inputs will be auto registered)
        * @param keyCode the keycode of the input
        * @param delegate method with void(InputValue)
        */
        void BindKeyboardInput(KeyCodes keyCode, const Delegate<void(InputValue)>& delegate);

        /**
        * Binds a keyboard input to a method (Unregistered inputs will be auto registered)
        * @param negativeKeyCode the keycode of the input, decides negative sign
        * @param positiveKeyCode the keycode of the input, decides positive sign
        * @param delegate method with void(InputValue)
        */
        void BindKeyboardAxisInput(KeyCodes negativeKeyCode, KeyCodes positiveKeyCode, const Delegate<void(InputValue)>& delegate);

        /**
        * Binds mouse movement input to a method
        * @param delegate method with void(MouseMoveValue)
        */
        void BindMouseMove(const Delegate<void(MouseMoveValue)>& delegate);

        /**
        * Binds a mouse button input to a method
        * @param buttonIndex MouseButton
        * @param delegate method with void(InputValue)
        */
        void BindMouseButton(MouseButton buttonIndex, const Delegate<void(InputValue)>& delegate);

        /**
        * Binds any mouse button input to a method
        * @param delegate method with void(MouseButtonValues)
        */
        void BindAnyMouseButton(const Delegate<void(MouseButtonValues)>& delegate);

        /**
        * Binds a mouse scroll to a method
        * @param delegate method with void(float, float)
        */
        void BindMouseScroll(const Delegate<void(Vector2)>& delegate);

        /**
        * Returns the trigger state of a key. If the requested key isn't registered returns only pressed or not pressed
        */
        TriggerState GetKeyTriggerState(int keyCode) const;
        // todo: just using for debug now. probably dirty to use array
        std::array<InputValue, 4> GetCurrentTriggeredKeys() const;

        /**
        * Returns current mouse pos
        */
        Vector2 GetAbsoluteMousePos() const;
        /**
        * Returns current mouse pos
        */
        Vector2 GetMousePos() const;
        /**
        * Returns current mouse delta
        */
        Vector2 GetMouseDelta() const;
        /**
        * Returns current mouse scroll deltas
        */
        Vector2 GetMouseScroll() const;
        /**
        * Returns current TriggerState for a mouse button
        */
        TriggerState GetMouseTriggerState(int index) const;
        /**
        * Returns current state of all mouse buttons
        */
        MouseButtonValues GetMouseButtonValues() const;

    protected:
        virtual void OnEvent(Event& e);
        bool IsValidAxisBindings(BindingPair negativeBindingPair, BindingPair positiveBindingPair);

        void OnKeyEvent(Event& e);
        void OnMouseEvent(Event& e);

    protected:
        InputHandler* _inputHandler;

        std::unordered_map<BindingPair, MulticastDelegate<void(InputValue)>> _actionMappingDelegates {};
        std::unordered_map<BindingPair, AxisBinding> _bindingToAxisBinding {};
        std::unordered_map<int, MulticastDelegate<void(InputValue)>> _axisActionMappingDelegates {};
        MulticastDelegate<void(MouseMoveValue)> _mouseMoveDelegate {};
        MulticastDelegate<void(MouseButtonValues)> _mouseButtonValueDelegate {};
        MulticastDelegate<void(Vector2)> _mouseScrollDelegate {};
    };
}
