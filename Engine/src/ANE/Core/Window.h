#pragma once
#include <SDL_events.h>
#include <SDL_video.h>

#include "ANE/Delegate/Delegate.h"
#include "ANE/Events/Event.h"

namespace Engine
{
    struct ANE_API WindowProperties
    {
        std::string Title;
        uint32_t Width;
        uint32_t Height;

        uint32_t XPos;
        uint32_t YPos;

        bool VSync;

        WindowProperties(const std::string& title = "ANE Engine", uint32_t width = 900, uint32_t height = 500, bool vSync = true) :
            Title(title), Width(width), Height(height), XPos(0), YPos(0), VSync(vSync)
        {
        }
    };

    class ANE_API Window
    {
    public:
        static std::unique_ptr<Window> Create(const WindowProperties& props = WindowProperties());

        Window(const WindowProperties& props);
        ~Window();

        template <class TClass>
        void BindOnEvent(DelegateMember<TClass, void(Event&)> delegateMember);

        void OnUpdate(float deltaTime);
        void SetVSync(bool enabled);

        bool HasFocus() const { return _imGuiHasFocus && _windowHasFocus; }

        bool HasWindowFocus() const { return _windowHasFocus; }
        bool ImGuiHasFocus() const { return _imGuiHasFocus; }

        bool IsVSync() const { return  _windowData.VSync; }
        uint32_t GetWidth() const { return _windowData.Width; }
        uint32_t GetHeight() const { return _windowData.Height; }
        WindowProperties GetProperties() { return _windowData; }
        SDL_Window* GetWindowContext() const { return _windowContext; }


    private:
        void Init(const WindowProperties& props);
        void Shutdown();

        void ProcessEvents(float deltaTime);
        void ProcessWindowEvent(const SDL_WindowEvent& windowEvent, float deltaTime);

        void DispatchEvent(Event& e);

    private:
        SDL_Window* _windowContext;

        WindowProperties _windowData;

        SinglecastDelegate<void(Event&)> _eventDelegate;

        bool _imGuiHasFocus = false;
        bool _windowHasFocus = false;

        //ImGuiIO* _io; Moved to VulkanRenderer, so expose getter functions there.
        bool _showDemoWindow = true;
        bool _showAnotherWindow = false;
    };

    template <class TClass>
    void Window::BindOnEvent(DelegateMember<TClass, void(Event&)> delegateMember)
    {
        _eventDelegate = delegateMember;
    }
}
