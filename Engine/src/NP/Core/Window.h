#pragma once
#include <imgui_impl_vulkan.h>
#include <SDL_events.h>
#include <SDL_video.h>
#include <vulkan/vulkan_core.h>

#include "imgui.h"
#include "../Delegate/Delegate.h"

//#include "SDL.h"
//#include "vulkan/vulkan_core.h"

namespace vkb
{
    struct Instance;
    struct PhysicalDevice;
    struct Device;
    struct Swapchain;
}

namespace Engine
{
    class VulkanRenderer;

    struct NP_API WindowProperties
    {
        std::string Title;
        uint32_t Width;
        uint32_t Height;

        bool VSync;

        WindowProperties(const std::string& title = "NP Engine", uint32_t width = 900, uint32_t height = 500, bool vSync = true) :
            Title(title), Width(width), Height(height), VSync(vSync)
        {
        }
    };

    struct QueueFamilyIndices
    {
        std::optional<uint32_t> GraphicsFamily;

        bool HasFamily() const
        {
            return GraphicsFamily.has_value();
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

        MulticastDelegate<void()> WindowCloseDelegate;
        MulticastDelegate<void(int width, int height)> WindowResizeDelegate;

    private:
        void Init(const WindowProperties& props);
        void Shutdown();

        void ProcessEvents(float deltaTime);
        void ProcessWindowEvent(const SDL_WindowEvent& windowEvent, float deltaTime);

    private:
        SDL_Window* _windowContext;

        WindowProperties _windowData;

        std::unique_ptr<VulkanRenderer> _vulkanRenderer;

        //ImGuiIO* _io; Moved to VulkanRenderer, so expose getter functions there.
        bool _showDemoWindow = true;
        bool _showAnotherWindow = false;
        ImVec4 _clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    };

}

