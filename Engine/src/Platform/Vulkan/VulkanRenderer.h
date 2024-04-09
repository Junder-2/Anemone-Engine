#pragma once
#include <SDL_video.h>
#include <imgui_impl_vulkan.h>
#include <span>

#include "VmaTypes.h"
#include "VulkanDeletionQueue.h"
#include "VulkanPipelineBuilder.h"

namespace vkb
{
    struct Instance;
    struct PhysicalDevice;
    struct Device;
    struct Swapchain;
}

namespace Engine
{
    struct WindowProperties;

    struct VulkanImmediateBuffer
    {
        VkFence Fence;
        VkCommandBuffer CommandBuffer;
        VkCommandPool CommandPool;
    };

    class VulkanRenderer
    {
        struct QueueFamilyIndices
        {
            std::optional<uint32_t> GraphicsFamily;

            bool HasFamily() const
            {
                return GraphicsFamily.has_value();
            }
        };
    public:
        VulkanRenderer(SDL_Window* window);

        void Setup();
        void NewFrame(const WindowProperties& props);
        void EndFrame();
        void Cleanup();

        float GetFramerate();
        static ImGuiIO* GetImGuiIO() { return _io; }

    private:
        // Vulkan
        static void CheckVkResult(VkResult err);

        static std::vector<const char*> GetAvailableExtensions(SDL_Window* window);

        static void SetupVulkan(SDL_Window* window);
        static void SetupImGui(SDL_Window* window);

        static vkb::Instance CreateVkbInstance(const std::vector<const char*>& extensions);
        static vkb::PhysicalDevice SelectVkbPhysicalDevice(VkSurfaceKHR surface, vkb::Instance instance);
        static vkb::Device CreateVkbLogicalDevice(const vkb::PhysicalDevice& physicalDevice);

        static void SetupCommandBuffers();

        static PipelineWrapper CreatePipeline(const vkb::Device& logicalDevice);

        static void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);

        static void CreateImGuiDescriptorPool();

        inline static void DrawImGui(VkCommandBuffer cmd, VkImageView targetImageView);

        inline static void RenderFrame(ImGui_ImplVulkanH_Window* wd, ImDrawData* drawData);
        inline static void RevealFrame(ImGui_ImplVulkanH_Window* wd);

        static void CleanupVulkan();
        static void CleanupImGui();

        // VMA
        // TODO: Isolate VMA code to separate file/class.
        static void CreateVmaAllocator();

        static VmaBuffer CreateBuffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);
        static void DestroyBuffer(const VmaBuffer& buffer);

        static VmaImage CreateImage(VkExtent3D size, VkFormat format, VkImageUsageFlags usage, bool mipmaps = false);
        static VmaImage CreateImage(const void* data, VkExtent3D size, VkFormat format, VkImageUsageFlags usage, bool mipmaps = false);
        static void DestroyImage(const VmaImage& image);

        static void ImmediateSubmit(std::function<void(VkCommandBuffer cmd)>&& function);

        static VmaMeshBuffers UploadMesh(std::span<uint32_t> indices, std::span<Vertex> vertices);

        // Debugging
        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

    public:
        inline static ImVec4 ClearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    private:
        inline static SDL_Window* _window;

        bool _initialized = false;

        // Vulkan
        #ifdef NDEBUG
        static constexpr bool enableValidationLayers = false;
        #else
        static constexpr bool enableValidationLayers = true;
        #endif

        // Deletion queue with the scope of the renderer.
        inline static VulkanDeletionQueue _mainDeletionQueue;

        inline static VkAllocationCallbacks* _allocator = nullptr;
        inline static VkInstance _instance = VK_NULL_HANDLE;
        inline static VkDebugUtilsMessengerEXT _debugMessenger;
        inline static VkSurfaceKHR _surface;
        inline static VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
        inline static VkDevice _device = VK_NULL_HANDLE;
        inline static QueueFamilyIndices _queueFamily = (QueueFamilyIndices)-1;
        inline static VkQueue _queue = VK_NULL_HANDLE;
        inline static VkPipelineCache _pipelineCache = VK_NULL_HANDLE;
        inline static VkPipeline _meshPipeline = VK_NULL_HANDLE;

        // VMA
        inline static VmaAllocator _vmaAllocator;

        inline static VulkanImmediateBuffer _immBuffer;

        // ImGui
        inline static ImGui_ImplVulkanH_Window _mainWindowData;
        inline static uint32_t _minImageCount = 2;
        inline static bool _swapChainRebuild = false;

        inline static VkDescriptorPool _imGuiDescriptorPool = VK_NULL_HANDLE;

        inline static ImGuiIO* _io;
    };
}
