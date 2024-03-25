#pragma once
#include <imgui_impl_vulkan.h>
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
        static ImVector<const char*> GetAvailableExtensions(SDL_Window* window);
        static bool CheckValidationLayers();
        static void CreateVulkanInstance(const ImVector<const char*>& extensions);
        static void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);
        static void CleanupVulkanWindow();
        static void CleanupVulkan();

    private:
        SDL_Window* _windowContext;

        WindowProperties _windowData;

        // Vulkan
        #ifdef NDEBUG
            static constexpr bool enableValidationLayers = false;
        #else
            static constexpr bool enableValidationLayers = true;
        #endif
        inline const static std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        inline static VkAllocationCallbacks* g_Allocator = nullptr;
        inline static VkInstance g_Instance = VK_NULL_HANDLE;
        inline static VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE;
        inline static VkDevice g_Device = VK_NULL_HANDLE;
        inline static uint32_t g_QueueFamily = (uint32_t)-1;
        inline static VkQueue g_Queue = VK_NULL_HANDLE;
        inline static VkPipelineCache g_PipelineCache = VK_NULL_HANDLE;
        inline static VkDescriptorPool g_DescriptorPool = VK_NULL_HANDLE;

        // ImGui
        inline static ImGui_ImplVulkanH_Window g_MainWindowData;
        inline static uint32_t g_MinImageCount = 2;

        ImGuiIO* _io;
        bool _showDemoWindow = true;
        bool _showAnotherWindow = false;
        ImVec4 _clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    };

}

