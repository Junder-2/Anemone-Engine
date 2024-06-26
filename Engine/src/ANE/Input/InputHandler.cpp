#include "anepch.h"
#include "InputHandler.h"

#include <ranges>
#include <SDL_keyboard.h>

#include "InputAction.h"
#include "InputTypes.h"
#include "ANE/Events/Event.h"

namespace Engine
{
    std::unique_ptr<InputHandler> InputHandler::Create()
    {
        return std::make_unique<InputHandler>();
    }

    InputHandler::InputHandler()
    {
        ANE_PROFILE_FUNCTION();

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

    void InputHandler::BindOnEvent(const Delegate<void(Event&)>& delegate)
    {
        _eventDelegate = delegate;
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
        ANE_DEEP_PROFILE_FUNCTION();

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
        ANE_PROFILE_FUNCTION();

        for (const auto val : _keyboardInputActions | std::views::values)
        {
            val->FlushAction();
        }

        _mouseInputAction.FlushAction();
        _currentKeyStates = nullptr;

        InputFlushEvent flushEvent;
        DispatchEvent(flushEvent);
    }

    void InputHandler::ProcessKey(const int keyCode, const bool press)
    {
        if (!_keyboardInputActions.contains(keyCode)) return;

        bool needProcessing = false;

        if(_keyboardInputActions[keyCode]->PopulateInput(ToFloat(press ? TriggerStarted : TriggerStopped), &needProcessing))
        {
            KeyboardKeyEvent keyTriggerEvent(_keyboardInputActions[keyCode]->GetInputValue());
            DispatchEvent(keyTriggerEvent);
        }

        if(needProcessing)
        {
            _dirtyKeys.push(keyCode);
        }
    }

    void InputHandler::ProcessAbsoluteMouseMovement(const Vector2 pos)
    {
        _mouseInputAction.PopulateAbsoluteMove(pos);
    }

    void InputHandler::ProcessMouseMovement(const Vector2 pos, const Vector2 delta)
    {
        if(_mouseInputAction.PopulateMoveInput(&_dirtyMouse, pos, delta))
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

    void InputHandler::ProcessMouseScroll(const Vector2 delta)
    {
        if(_mouseInputAction.PopulateScrollInput(&_dirtyMouse, delta))
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

    MouseInputAction InputHandler::GetMouseInputData() const
    {
        return _mouseInputAction;
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

        return TriggerNone;
    }
}
