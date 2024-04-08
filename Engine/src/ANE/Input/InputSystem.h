#pragma once
#include "InputAction.h"
#include "InputHandler.h"

namespace Engine
{
    //Todo: use a abstract service locator
    class InputSystem
    {
    public:
        // TEMP
        static void Create();
        static InputSystem& Get() { return *_instance; }
        //

        InputSystem();
        ~InputSystem() = default;

        void OnEvent(Event& e);

        template <class TClass>
        void BindInput(InputDeviceType deviceType, int bindingId, DelegateMember<TClass, void(InputValue)> delegateMember);

        TriggerState GetKeyTriggerState(const int keyCode) const { return _inputHandler->GetKeyTriggerState(keyCode); }
        // todo: just using for debug now. probably dirty to use array
        std::array<InputValue, 4> GetCurrentTriggeredKeys() const { return _inputHandler->GetCurrentTriggeredKeys(); }

        glm::vec2 GetMouseDelta() const { return _inputHandler->GetMouseInputData().GetMoveValue().GetMouseDelta(); }
        glm::vec2 GetMousePos() const { return _inputHandler->GetMouseInputData().GetMoveValue().GetMousePos(); }
        TriggerState GetMouseTriggerState(const int index) const { return _inputHandler-> GetMouseInputData().GetButtonValue().GetTriggerState(index); }
        MouseButtonValues GetMouseButtonValues() const { return _inputHandler->GetMouseInputData().GetButtonValue(); }

    private:
        static std::unique_ptr<InputSystem> _instance;
        InputHandler* _inputHandler;

        std::unordered_map<IntPair, MulticastDelegate<void(InputValue)>> _actionMappings;
    };

    template <class TClass>
    void InputSystem::BindInput(const InputDeviceType deviceType, const int bindingId, DelegateMember<TClass, void(InputValue)> delegateMember)
    {
        const IntPair bindingPair(deviceType, bindingId);

        switch(deviceType)
        {
            case InputDeviceKeyboard:
                _inputHandler->RegisterKeyboardKey(bindingId);
            break;
        }

        _actionMappings[bindingPair] += delegateMember;
    }
}
