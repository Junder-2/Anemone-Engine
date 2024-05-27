#pragma once
#include <entt.hpp>
#include <SDL_events.h>
#include <SDL_video.h>

#include "ANE/Math/Types/Vector2.h"
#include "ANE/Delegate/Delegate.h"

namespace Engine
{
    class Event;

    struct WindowProperties
    {
        std::string Title;
        uint Width;
        uint Height;

        uint XPos;
        uint YPos;

        uint Id = 0;
        bool VSync;

        WindowProperties(const std::string& title = "ANE Engine", const uint width = 900, const uint height = 500, const bool vSync = true) :
            Title(title), Width(width), Height(height), XPos(0), YPos(0), VSync(vSync) {}
    };

    // ImGuiViewport
    struct ViewportProperties
    {
        ViewportProperties() = default;
        ViewportProperties(const uint id, const uint width, const uint height, const uint xPos = 0, const uint yPos = 0) : Id(id),  Width(width), Height(height), XPos(xPos), YPos(yPos) {}

        uint Id = 0;

        uint Width = 0;
        uint Height = 0;

        uint XPos = 0;
        uint YPos = 0;

        Vector2 GetCenter() const { return {(float)Width * 0.5f + (float)XPos, (float)Height * 0.5f + (float)YPos}; }

        ViewportProperties& operator=(const WindowProperties& props)
        {
            Id = props.Id;
            Width = props.Width;
            Height = props.Height;
            XPos = 0;
            YPos = 0;
            return *this;
        }
    };

    class Window
    {
    public:
        static std::unique_ptr<Window> Create(const WindowProperties& props = WindowProperties());

        Window(const WindowProperties& props);
        ~Window();

        void BindOnEvent(const Delegate<void(Event&)>& delegate);

        void OnUpdate(float deltaTime);
        void SetVSync(bool enabled);

        void SetActiveViewport(uint id);
        void AddViewport(uint id);
        void RemoveViewport(uint id);

        static void SetMouseVisibility(bool enable);
        static bool IsMouseVisible();

        bool HasFocus() const { return !_imGuiHasFocus && _windowHasFocus; }

        bool HasWindowFocus() const { return _windowHasFocus; }
        bool ImGuiHasFocus() const { return _imGuiHasFocus; }

        bool IsVSync() const { return  _windowData.VSync; }
        uint GetWindowWidth() const { return _windowData.Width; }
        uint GetWindowHeight() const { return _windowData.Height; }
        WindowProperties GetWindowProperties() const { return _windowData; }
        SDL_Window* GetWindowContext() const { return _windowContext; }

        ViewportProperties GetActiveViewportProperties() const;
        uint GetViewportId() const { return _activeViewportId; }
        bool IsViewportMainWindow() const { return _activeViewportId == _windowData.Id; }
        bool IsOverViewport() const;

    private:
        void Init(const WindowProperties& props);
        void Shutdown();

        void ProcessEvents(float deltaTime);
        void ProcessWindowEvent(const SDL_WindowEvent& windowEvent, float deltaTime);
        void ProcessViewportEvents(const ViewportProperties& previousProps);

        Vector2 GetAbsoluteMousePos() const;

        void DispatchEvent(Event& e);

    private:
        SDL_Window* _windowContext;
        WindowProperties _windowData;

        uint _activeViewportId = 0;
        uint _previousViewportId = 0;
        entt::dense_set<uint> _viewports;
        ViewportProperties _previousViewportProperties {};

        SinglecastDelegate<void(Event&)> _eventDelegate;

        bool _imGuiHasFocus = false;
        bool _windowHasFocus = false;
    };
}
