#include "anepch.h"
#include "MouseInputAction.h"

namespace Engine
{
    MouseInputAction::MouseInputAction() = default;

    MouseInputAction::~MouseInputAction() = default;

    void MouseInputAction::PopulateAbsoluteMove(const Vector2 pos)
    {
        _moveValue.SetAbsoluteMousePos(pos);
    }

    bool MouseInputAction::PopulateMoveInput(bool* needProcessing, const Vector2 pos, const Vector2 delta)
    {
        _moveValue.SetMousePos(pos);
        _moveValue.SetMouseDelta(delta);

        *needProcessing = true;
        return true;
    }

    bool MouseInputAction::PopulateButtonInput(bool* needProcessing, const int buttonIndex, const TriggerState newState, const bool isDoubleClick /*= false */)
    {
        _buttonValue.SetTriggerState(buttonIndex, newState, isDoubleClick);

        *needProcessing = true;
        return true;
    }

    bool MouseInputAction::PopulateScrollInput(bool* needProcessing, const Vector2 delta)
    {
        _scrollWheelValue = delta;

        *needProcessing = true;
        return true;
    }

    bool MouseInputAction::ProcessAction()
    {
        bool changed = false;
        _moveValue.SetMouseDelta(Vector2::ZeroVector());
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
        _moveValue.SetMouseDelta(Vector2::ZeroVector());

        for (int i = 0; i < MOUSE_BUTTON_MAX; ++i)
        {
            const auto prevValue = _buttonValue.GetTriggerState(i);
            if(prevValue == TriggerNone) continue;
            _buttonValue.SetTriggerState(i, TriggerNone);
            changed = true;
        }

        return changed;
    }
}
