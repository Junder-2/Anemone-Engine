#pragma once
#include <SDL_events.h>
#include <SDL_video.h>

#include "../Delegate/Delegate.h"
#include "../Events/Event.h"

//#include "SDL.h"
//#include "vulkan/vulkan_core.h"

namespace Engine
{
    class VulkanRenderer;

    struct NP_API WindowProperties
    {
        std::string Title;
        uint32_t Width;
        uint32_t Height;

        uint32_t XPos;
        uint32_t YPos;

        bool VSync;

        WindowProperties(const std::string& title = "NP Engine", uint32_t width = 900, uint32_t height = 500, bool vSync = true) :
            Title(title), Width(width), Height(height), XPos(0), YPos(0), VSync(vSync)
        {
        }
    };

    class NP_API Window
    {
    public:
        static std::unique_ptr<Window> Create(const WindowProperties& props = WindowProperties());

        Window(const WindowProperties& props);
        ~Window();

        void OnUpdate(float deltaTime);
        void SetVSync(bool enabled);

        bool IsVSync() const { return  _windowData.VSync; }
        uint32_t GetWidth() const { return _windowData.Width; }
        uint32_t GetHeight() const { return _windowData.Height; }
        SDL_Window* GetWindowContext() const { return _windowContext; }

        SinglecastDelegate<void(Event&)> EventDelegate;

    private:
        void Init(const WindowProperties& props);
        void Shutdown();

        void ProcessEvents(float deltaTime);
        void ProcessWindowEvent(const SDL_WindowEvent& windowEvent, float deltaTime);

        void DispatchEvent(Event& e);

        static void ShowInputDebugOverlay(bool* pOpen);

    private:
        SDL_Window* _windowContext;

        WindowProperties _windowData;

        std::unique_ptr<VulkanRenderer> _vulkanRenderer;

        bool LostFocus() const { return _imGuiLostFocus || _windowLostFocus; }
        bool _imGuiLostFocus = false;
        bool _windowLostFocus = false;

        //ImGuiIO* _io; Moved to VulkanRenderer, so expose getter functions there.
        bool _showDemoWindow = true;
        bool _showAnotherWindow = false;
    };

}

