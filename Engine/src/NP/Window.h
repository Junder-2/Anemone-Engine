#pragma once
#include <SDL_render.h>
#include <SDL_video.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan_core.h>

#include "imgui.h"
#include "Delegate/Delegate.h"

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

        void OnUpdate();
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

        // ImGui
        static void CleanupImGui();

        // Vulkan
        static void CheckVKResult(VkResult err);
        static bool CheckValidationLayers();
        static void CreateVulkanInstance(const ImVector<const char*>& extensions);
        static void CleanupVulkanWindow();
        static void CleanupVulkan();

    private:
        SDL_Window* _windowContext;
        SDL_Renderer* _renderer;
        VkDevice _vkDevice;
        VkInstance _vkInstance;

        WindowProperties _windowData;

        // Vulkan
        inline static VkDevice _device = VK_NULL_HANDLE;
        #ifdef NDEBUG
            static constexpr bool enableValidationLayers = false;
        #else
            static constexpr bool enableValidationLayers = true;
        #endif
        inline const static std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        ImGuiIO* _io;
        bool _showDemoWindow = true;
        bool _showAnotherWindow = false;
        ImVec4 _clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    };

}

