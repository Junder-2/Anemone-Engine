#pragma once
#include <entt.hpp>
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

        uint32_t Id = 0;
        bool VSync;

        WindowProperties(const std::string& title = "ANE Engine", const uint32_t width = 900, const uint32_t height = 500, const bool vSync = true) :
            Title(title), Width(width), Height(height), XPos(0), YPos(0), VSync(vSync) {}
    };

    // ImGuiViewport
    struct ANE_API ViewportProperties
    {
        ViewportProperties() = default;
        ViewportProperties(const uint32_t id, const uint32_t width, const uint32_t height, const uint32_t xPos = 0, const uint32_t yPos = 0) : Id(id),  Width(width), Height(height), XPos(xPos), YPos(yPos) {}

        uint32_t Id;

        uint32_t Width;
        uint32_t Height;

        uint32_t XPos;
        uint32_t YPos;

        Vector2 GetCenter() const { return {(float)Width * 0.5f + (float)XPos, (float)Height * 0.5f + (float)YPos}; }

        ViewportProperties operator=(const WindowProperties& props)
        {
            Id = props.Id;
            Width = props.Width;
            Height = props.Height;
            XPos = 0;
            YPos = 0;
            return *this;
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

        void SetActiveViewport(uint32_t id);
        void SetActiveViewport(const ViewportProperties& props);
        void AddViewport(const ViewportProperties& props);
        void RemoveViewport(uint32_t id);

        void SetMouseVisibility(bool enable);

        bool HasFocus() const { return !_imGuiHasFocus && _windowHasFocus; }

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

        Vector2 GetAbsoluteMousePos() const;

        void DispatchEvent(Event& e);

    private:
        SDL_Window* _windowContext;
        WindowProperties _windowData;

        ViewportProperties* _activeViewport;
        entt::dense_map<uint32_t, ViewportProperties> _viewports;

        SinglecastDelegate<void(Event&)> _eventDelegate;

        bool _imGuiHasFocus = false;
        bool _windowHasFocus = false;
    };

    template <class TClass>
    void Window::BindOnEvent(DelegateMember<TClass, void(Event&)> delegateMember)
    {
        _eventDelegate = delegateMember;
    }
}
