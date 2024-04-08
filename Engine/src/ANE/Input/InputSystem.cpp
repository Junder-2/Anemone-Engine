#include "anepch.h"
#include "InputSystem.h"

#include "ANE/Core/Application.h"
#include "ANE/Events/EventHandler.h"

namespace Engine
{
    std::unique_ptr<InputSystem> InputSystem::_instance = nullptr;

    void InputSystem::Create()
    {
        _instance = std::make_unique<InputSystem>();
    }

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

            const IntPair bindingPair(keyValue.GetDeviceType(), keyValue.GetBindingId());

            if(_actionMappingDelegates.contains(bindingPair))
            {
                _actionMappingDelegates[bindingPair](keyValue);
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

                    const IntPair bindingPair(InputDeviceMouse, buttonId);

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
}
