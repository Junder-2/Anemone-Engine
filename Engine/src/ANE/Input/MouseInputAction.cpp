#include "anepch.h"
#include "MouseInputAction.h"

namespace Engine
{
    MouseInputAction::MouseInputAction() = default;

    MouseInputAction::~MouseInputAction() = default;

    bool MouseInputAction::PopulateMoveInput(bool* needProcessing, const float x, const float y, const float deltaTime)
    {
        const Vector2 prevMousePos = _moveValue.GetMousePos();
        _moveValue.SetMousePos(x, y);
        _moveValue.SetMouseDelta((x - prevMousePos.X)*deltaTime, (y - prevMousePos.Y)*deltaTime);

        *needProcessing = true;
        return true;
    }

    bool MouseInputAction::PopulateMoveInput(bool* needProcessing, const float x, const float y, const float xDelta, const float yDelta)
    {
        _moveValue.SetMousePos(x, y);
        _moveValue.SetMouseDelta(xDelta, yDelta);

        *needProcessing = true;
        return true;
    }

    bool MouseInputAction::PopulateButtonInput(bool* needProcessing, const int buttonIndex, const TriggerState newState, const bool isDoubleClick /*= false */)
    {
        _buttonValue.SetTriggerState(buttonIndex, newState, isDoubleClick);

        *needProcessing = true;
        return true;
    }

    bool MouseInputAction::PopulateScrollInput(bool* needProcessing, const float x, const float y)
    {
        _scrollWheelValue.X = x;
        _scrollWheelValue.Y = y;

        *needProcessing = true;
        return true;
    }

    bool MouseInputAction::ProcessAction()
    {
        bool changed = false;
        _moveValue.SetMouseDelta(0, 0);
        _scrollWheelValue.X = 0;
        _scrollWheelValue.Y = 0;

        for (int i = 0; i < MOUSE_BUTTON_MAX; ++i)
        {
            switch (_buttonValue.GetTriggerState(i))
            {
            case TriggerStarted:
                _buttonValue.SetTriggerState(i, TriggerHolding);
                changed = true;
            break;
            case TriggerHolding:
                changed = true;
            break;
            case TriggerStopped:
                _buttonValue.SetTriggerState(i, TriggerNone);
                changed = true;
            break;
            }
        }

        return changed;
    }

    bool MouseInputAction::FlushAction()
    {
        bool changed = false;
        _moveValue.SetMouseDelta(0, 0);

        for (int i = 0; i < MOUSE_BUTTON_MAX; ++i)
        {
            auto prevValue = _buttonValue.GetTriggerState(i);
            if(prevValue == TriggerNone) continue;
            _buttonValue.SetTriggerState(i, TriggerNone);
            changed = true;
        }

        return changed;
    }
}
