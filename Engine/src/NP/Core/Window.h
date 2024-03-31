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

        // ImGui
        static void CleanupImGui();

        // Vulkan
        static void CheckVkResult(VkResult err);

        static void SetupVulkan(SDL_Window* window);

        static std::vector<const char*> GetAvailableExtensions(SDL_Window* window);
        static bool CheckValidationLayers();

        static vkb::Instance CreateVkbInstance(const std::vector<const char*>& extensions);
        static vkb::PhysicalDevice SelectVkbPhysicalDevice(VkSurfaceKHR surface, vkb::Instance instance);
        static vkb::Device CreateVkbLogicalDevice(const vkb::PhysicalDevice& physicalDevice);

        static VkPhysicalDevice SelectPhysicalDevice();
        static bool IsDeviceCompatible(const VkPhysicalDevice& device);
        static int GetDeviceScore(const VkPhysicalDevice& device);

        static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

        static void CreateLogicalDevice();

        static void CreateDescriptorPool();

        static void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);

        inline static void RenderFrame(ImGui_ImplVulkanH_Window* wd, ImDrawData* drawData);
        inline static void RevealFrame(ImGui_ImplVulkanH_Window* wd);

        static void CleanupVulkan();

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

    private:
        SDL_Window* _windowContext;

        WindowProperties _windowData;

        // Vulkan
        std::unique_ptr<VulkanRenderer> _vulkanRenderer;

        #ifdef NDEBUG
            static constexpr bool enableValidationLayers = false;
        #else
            static constexpr bool enableValidationLayers = true;
        #endif
        inline const static std::vector<const char*> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
        };

        inline static VkAllocationCallbacks* _allocator = nullptr;
        inline static VkInstance _instance = VK_NULL_HANDLE;
        inline static VkDebugUtilsMessengerEXT _debugMessenger;
        inline static VkSurfaceKHR _surface;
        inline static VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
        inline static VkDevice _device = VK_NULL_HANDLE;
        inline static QueueFamilyIndices _queueFamily = (QueueFamilyIndices)-1;
        inline static VkQueue _queue = VK_NULL_HANDLE;
        inline static VkPipelineCache _pipelineCache = VK_NULL_HANDLE;
        inline static VkDescriptorPool _descriptorPool = VK_NULL_HANDLE;

        // ImGui
        inline static ImGui_ImplVulkanH_Window _mainWindowData;
        inline static uint32_t _minImageCount = 2;
        inline static bool _swapChainRebuild = false;

        ImGuiIO* _io;
        bool _showDemoWindow = true;
        bool _showAnotherWindow = false;
        ImVec4 _clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    };

}

