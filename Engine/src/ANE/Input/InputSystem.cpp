#include "anepch.h"
#include "InputSystem.h"

#include "InputHandler.h"
#include "ANE/Core/Application.h"
#include "ANE/Events/Event.h"
#include "ANE/Events/EventHandler.h"
#include "ANE/Utilities/InputUtilities.h"

namespace Engine
{
    InputSystem::InputSystem()
    {
        _inputHandler = &Application::Get().GetInputHandler();

        EventHandler::BindAppEvent(MakeDelegate(this, &InputSystem::OnEvent));
    }

    void InputSystem::BindInput(const BindingPair bindingPair, const Delegate<void(InputValue)>& delegate)
    {
        switch(bindingPair.DeviceType)
        {
            case InputDeviceKeyboard:
                _inputHandler->RegisterKeyboardKey(bindingPair.BindingId);
            break;
        }

        _actionMappingDelegates[bindingPair] += delegate;
    }

    void InputSystem::BindKeyboardInput(const KeyCodes keyCode, const Delegate<void(InputValue)>& delegate)
    {
        BindInput(BindingPair(InputDeviceKeyboard, keyCode), delegate);
    }

    void InputSystem::BindMouseButton(const MouseButton buttonIndex, const Delegate<void(InputValue)>& delegate)
    {
        BindInput(BindingPair(InputDeviceMouse, buttonIndex), delegate);
    }

    void InputSystem::BindMouseMove(const Delegate<void(MouseMoveValue)>& delegate)
    {
        _mouseMoveDelegate += delegate;
    }

    void InputSystem::BindAnyMouseButton(const Delegate<void(MouseButtonValues)>& delegate)
    {
        _mouseButtonValueDelegate += delegate;
    }

    void InputSystem::BindMouseScroll(const Delegate<void(Vector2)>& delegate)
    {
        _mouseScrollDelegate += delegate;
    }

    void InputSystem::BindAxisInput(const BindingPair negativeBindingPair, const BindingPair positiveBindingPair, const Delegate<void(InputValue)>& delegate)
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

        _axisActionMappingDelegates[id] += delegate;
    }

    void InputSystem::BindKeyboardAxisInput(const KeyCodes negativeKeyCode, const KeyCodes positiveKeyCode, const Delegate<void(InputValue)>& delegate)
    {
        BindAxisInput({InputDeviceKeyboard, negativeKeyCode}, {InputDeviceKeyboard, positiveKeyCode}, delegate);
    }

    void InputSystem::OnEvent(Event& e)
    {
        if(!e.HasCategory(EventCategoryInput)) return;

        if(e.HasCategory(EventCategoryKeyboard))
        {
            const InputValue keyValue = dynamic_cast<KeyboardKeyEvent&>(e).GetInputValue();

            const BindingPair bindingPair(keyValue.GetDeviceType(), keyValue.GetBindingId());

            if(_actionMappingDelegates.contains(bindingPair))
            {
                _actionMappingDelegates[bindingPair](keyValue);
            }

            if(!_bindingToAxisBinding.empty())
            {
                const AxisBinding axisBinding = _bindingToAxisBinding[bindingPair];

                InputValue axisValue(InputTypeAxis, (InputDeviceType)bindingPair.DeviceType, keyValue.GetBindingId());

                const float sign = axisBinding.Sign ? 1.f : -1.f;
                switch (keyValue.GetValueType())
                {
                    case InputTypeTrigger:
                        axisValue = keyValue.GetTriggerState() != TriggerStopped ? sign : 0;
                        break;
                    case InputTypeBoolean:
                        axisValue = keyValue.GetIntValue() != 0 ? sign : 0;
                        break;
                    case InputTypeAxis:
                        axisValue = keyValue.GetAxis() * sign;
                        break;
                }

                _axisActionMappingDelegates[axisBinding.Id](axisValue);
            }
        }

        if(e.HasCategory(EventCategoryMouse))
        {
            switch (e.GetEventType())
            {
                case EventType::MouseMovement:
                {
                    const MouseMoveValue mouseMoveValue = dynamic_cast<MouseMovementEvent&>(e).GetInputValue();

                    if(_mouseMoveDelegate) _mouseMoveDelegate(mouseMoveValue);
                }
                break;
                case EventType::MouseScrolled:
                {
                    const MouseScrollEvent mouseScrollEvent = dynamic_cast<MouseScrollEvent&>(e);

                    if(_mouseScrollDelegate) _mouseScrollDelegate({mouseScrollEvent.GetXDelta(), mouseScrollEvent.GetYDelta()});
                }
                break;
                case EventType::MouseButton:
                {
                    const MouseButtonValues mouseButtonValues = dynamic_cast<MouseButtonEvent&>(e).GetInputValue();

                    if(_mouseButtonValueDelegate) _mouseButtonValueDelegate(mouseButtonValues);

                    const int buttonId = mouseButtonValues.GetCurrentButtonIndex();

                    const BindingPair bindingPair(InputDeviceMouse, buttonId);

                    if(_actionMappingDelegates.contains(bindingPair))
                    {
                        const InputValue buttonValue(mouseButtonValues.GetTriggerState(), InputTypeTrigger, InputDeviceMouse, buttonId);

                        _actionMappingDelegates[bindingPair](buttonValue);
                    }
                }
                break;
            }
        }
    }

    bool InputSystem::IsValidAxisBindings(const BindingPair negativeBindingPair, const BindingPair positiveBindingPair)
    {
        // TODO: Current system doesnt allow one binding pair to be bound to more than one other binding pair
        if(_bindingToAxisBinding.contains(positiveBindingPair) && !_bindingToAxisBinding.contains(negativeBindingPair))
        {
            ANE_ELOG_WARN("Trying to bind existing positive binding to different axis, this is not currently supported: ({0}, {1})"
                                , InputUtilities::ToString((InputDeviceType)positiveBindingPair.DeviceType), positiveBindingPair.BindingId);
            return true;
        }
        else if(!_bindingToAxisBinding.contains(positiveBindingPair) && _bindingToAxisBinding.contains(negativeBindingPair))
        {
            ANE_ELOG_WARN("Trying to bind existing negative binding to different axis, this is not currently supported: ({0}, {1})"
                                , InputUtilities::ToString((InputDeviceType)negativeBindingPair.DeviceType), negativeBindingPair.BindingId);
            return true;
        }
        else if(_bindingToAxisBinding[positiveBindingPair].Id != _bindingToAxisBinding[negativeBindingPair].Id)
        {
            ANE_ELOG_WARN("Trying to bind existing binding to different axis, this is not currently supported: ({0}, {1}) and ({2}, {3})"
                                , InputUtilities::ToString((InputDeviceType)positiveBindingPair.DeviceType), positiveBindingPair.BindingId
                                , InputUtilities::ToString((InputDeviceType)negativeBindingPair.DeviceType), negativeBindingPair.BindingId);
            return true;
        }
        return false;
    }

    TriggerState InputSystem::GetKeyTriggerState(const int keyCode) const
    {
        return _inputHandler->GetKeyTriggerState(keyCode);
    }

    std::array<InputValue, 4> InputSystem::GetCurrentTriggeredKeys() const
    {
        return _inputHandler->GetCurrentTriggeredKeys();
    }

    Vector2 InputSystem::GetAbsoluteMousePos() const
    {
        return _inputHandler->GetMouseInputData().GetMoveValue().GetAbsoluteMousePos();
    }

    Vector2 InputSystem::GetMousePos() const
    {
        return _inputHandler->GetMouseInputData().GetMoveValue().GetMousePos();
    }

    Vector2 InputSystem::GetMouseDelta() const
    {
        return _inputHandler->GetMouseInputData().GetMoveValue().GetMouseDelta();
    }

    Vector2 InputSystem::GetMouseScroll() const
    {
        return _inputHandler->GetMouseInputData().GetScrollValue();
    }

    TriggerState InputSystem::GetMouseTriggerState(const int index) const
    {
        return _inputHandler-> GetMouseInputData().GetButtonValue().GetTriggerState(index);
    }

    MouseButtonValues InputSystem::GetMouseButtonValues() const
    {
        return _inputHandler->GetMouseInputData().GetButtonValue();
    }
}
