#include "anepch.h"
#include "InputSystem.h"

#include "ANE/Core/Application.h"
#include "ANE/Events/EventHandler.h"
#include "ANE/Utilities/InputUtilities.h"

namespace Engine
{
    InputSystem::InputSystem()
    {
        _inputHandler = &Application::Get().GetInputHandler();

        EventHandler::BindAppEvent(MakeDelegate(this, &InputSystem::OnEvent));
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

                    if(_mouseScrollDelegate) _mouseScrollDelegate(mouseScrollEvent.GetXDelta(), mouseScrollEvent.GetYDelta());
                }
                break;
                case EventType::MouseButton:
                {
                    const MouseButtonValues mouseButtonValues = dynamic_cast<MouseButtonEvent&>(e).GetInputValue();

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
}
