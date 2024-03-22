#pragma once
#include <SDL_video.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan_core.h>

//#include "SDL.h"
//#include "vulkan/vulkan_core.h"

namespace Engine
{
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

    class NP_API Window
    {
    public:
        static std::unique_ptr<Window> Create(const WindowProperties& props = WindowProperties());

        Window(const WindowProperties& props);
        ~Window();

        bool OnUpdate();
        void SetVSync(bool enabled);

        bool IsVSync() const { return  _windowData.VSync; }
        uint32_t GetWidth() const { return _windowData.Width; }
        uint32_t GetHeight() const { return _windowData.Height; }
        SDL_Window* GetWindowContext() const { return _windowContext; }

    private:
        void Init(const WindowProperties& props);
        void Shutdown();
    private:
        SDL_Window* _windowContext;
        VkDevice _vkDevice;
        VkInstance _vkInstance;

        WindowProperties _windowData;
    };

}

