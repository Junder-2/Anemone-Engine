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

            if(_actionMappings.contains(bindingPair))
            {
                _actionMappings[bindingPair](keyValue);
            }
        }
    }
}
