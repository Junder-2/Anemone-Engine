#include "anepch.h"
#include "InputHandler.h"

#include <ranges>
#include <SDL_keyboard.h>

#include "InputAction.h"
#include "InputTypes.h"

namespace Engine
{
    std::unique_ptr<InputHandler> InputHandler::Create()
    {
        return std::make_unique<InputHandler>();
    }

    InputHandler::InputHandler()
    {
        // input testing
        for (int i = KeyCodeA; i < KeyCodeZ+1; ++i)
        {
            RegisterKeyboardKey(i);
        }

        for (int i = KeyCode0; i < KeyCode9+1; ++i)
        {
            RegisterKeyboardKey(i);
        }
    }

    InputHandler::~InputHandler()
    {
        for (const InputAction* val : _keyboardInputActions | std::views::values)
        {
            delete val;
        }

        _keyboardInputActions.clear();
    }

    void InputHandler::DispatchEvent(Event& e)
    {
        if(_eventDelegate) _eventDelegate(e);
    }

    void InputHandler::RegisterKeyboardKey(const int keyCode)
    {
        if (_keyboardInputActions.contains(keyCode)) return;

        InputTrigger* newInputAction = new InputTrigger(InputDeviceKeyboard, keyCode);

        _keyboardInputActions.insert_or_assign(keyCode, newInputAction);
    }

    void InputHandler::OnUpdate()
    {
        bool needProcessing = false;

        if(_dirtyMouse)
        {
            if(_mouseInputAction.ProcessAction())
            {
                MouseButtonEvent mouseButtonEvent(_mouseInputAction.GetButtonValue());
                DispatchEvent(mouseButtonEvent);
            }
            else
            {
                _dirtyMouse = false;
            }
        }

        if(!_dirtyKeys.empty())
        {
            std::queue<int> dirtyKeysCopy;
            std::swap(_dirtyKeys, dirtyKeysCopy);

            while(!dirtyKeysCopy.empty())
            {
                needProcessing = false;
                int keyCode = dirtyKeysCopy.front();
                if(_keyboardInputActions[keyCode]->ProcessAction(&needProcessing))
                {
                    KeyboardKeyEvent keyTriggerEvent(_keyboardInputActions[keyCode]->GetInputValue());
                    DispatchEvent(keyTriggerEvent);
                }
                if(needProcessing)
                {
                    _dirtyKeys.push(keyCode);
                }
                dirtyKeysCopy.pop();
            }
        }
    }

    void InputHandler::PopulateKeyStates(const Uint8* newKeyStates)
    {
        _currentKeyStates = newKeyStates;
    }

    void InputHandler::FlushInputs()
    {
        for (const auto val : _keyboardInputActions | std::views::values)
        {
            if(val->FlushAction())
            {
                KeyboardKeyEvent keyTriggerEvent(val->GetInputValue());
                DispatchEvent(keyTriggerEvent);
            }
        }

        _mouseInputAction.FlushAction();
        _currentKeyStates = nullptr;
    }

    void InputHandler::ProcessKey(const int keyCode, const bool press)
    {
        if (!_keyboardInputActions.contains(keyCode)) return;

        bool needProcessing = false;

        if(_keyboardInputActions[keyCode]->PopulateInput((float)(press ? TriggerStarted : TriggerStopped), &needProcessing))
        {
            KeyboardKeyEvent keyTriggerEvent(_keyboardInputActions[keyCode]->GetInputValue());
            DispatchEvent(keyTriggerEvent);
        }

        if(needProcessing)
        {
            _dirtyKeys.push(keyCode);
        }
    }

    void InputHandler::ProcessMouseMovement(const float xPos, const float yPos, const float deltaTime)
    {
        if(_mouseInputAction.PopulateMoveInput(&_dirtyMouse, xPos, yPos, deltaTime))
        {
            MouseMovementEvent mouseMovementEvent(_mouseInputAction.GetMoveValue());
            DispatchEvent(mouseMovementEvent);
        }
    }

    void InputHandler::ProcessMouseButton(const int index, const bool press, const bool isDoubleClick /*= false */)
    {
        if(_mouseInputAction.PopulateButtonInput(&_dirtyMouse, index, press ? TriggerStarted : TriggerStopped, isDoubleClick))
        {
            MouseButtonEvent mouseButtonEvent(_mouseInputAction.GetButtonValue());
            DispatchEvent(mouseButtonEvent);
        }
    }

    void InputHandler::ProcessMouseScroll(const float xDelta, const float yDelta)
    {
        if(_mouseInputAction.PopulateScrollInput(&_dirtyMouse, xDelta, yDelta))
        {
            MouseScrollEvent mouseWheelEvent(_mouseInputAction.GetScrollValue());
            DispatchEvent(mouseWheelEvent);
        }
    }

    std::array<InputValue, 4> InputHandler::GetCurrentTriggeredKeys()
    {
        std::array<InputValue, 4> newArray;

        int index = 0;

        for (const InputAction* val : _keyboardInputActions | std::views::values)
        {
            if (val->GetInputValue().GetIntValue() == 0) continue;

            newArray[index] = val->GetInputValue();
            index++;
            if(index >= 4) break;
        }

        return newArray;
    }

    TriggerState InputHandler::GetKeyTriggerState(const int keyCode)
    {
        if (_keyboardInputActions.contains(keyCode))
        {
            return _keyboardInputActions[keyCode]->GetInputValue().GetTriggerState();
        }

        if (_currentKeyStates != nullptr && _currentKeyStates[SDL_GetScancodeFromKey(keyCode)])
        {
            return TriggerHolding;
        }

        //ANE_ELOG_WARN("No registered of key {0}", keyCode);

        return TriggerNone;
    }
}
