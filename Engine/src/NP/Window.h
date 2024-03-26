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

// Proxy functions
inline VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);

    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

inline void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

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

        static VkPhysicalDevice SelectPhysicalDevice();
        static bool IsDeviceCompatible(const VkPhysicalDevice& device);
        static int GetDeviceScore(const VkPhysicalDevice& device);

        static QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

        static void CreateLogicalDevice();

        static void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);

        static void CleanupVulkanWindow();
        static void CleanupVulkan();

        static void SetupDebugMessenger();
        static void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

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
        inline static VkDebugUtilsMessengerEXT g_DebugMessenger;
        inline static VkPhysicalDevice g_PhysicalDevice = VK_NULL_HANDLE;
        inline static VkDevice g_Device = VK_NULL_HANDLE;
        inline static QueueFamilyIndices g_QueueFamily = (QueueFamilyIndices)-1;
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

