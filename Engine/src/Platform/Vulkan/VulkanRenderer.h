#pragma once
#include <entt.hpp>
#include <SDL_video.h>
#include <imgui_impl_vulkan.h>
#include <span>

#include "VmaTypes.h"
#include "VulkanDeletionQueue.h"
#include "VulkanDescriptors.h"
#include "VulkanPipelineBuilder.h"
#include "ANE/Core/Math/Matrix/Matrix4x4.h"
#include "ANE/Renderer/Draw.h"
#include "ANE/Renderer/Mesh.h"

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

    struct VulkanFrame
    {
        VkCommandPool CommandPool;
        VkCommandBuffer CommandBuffer;

        VkSemaphore SwapchainSemaphore, RenderSemaphore;
        VkFence Fence;

        VkImageView BackbufferView;
        VkFramebuffer Framebuffer;

        VulkanDeletionQueue DeletionQueue;
        DescriptorAllocator Descriptors;
    };

    struct VulkanImmediateBuffer
    {
        VkFence Fence;
        VkCommandBuffer CommandBuffer;
        VkCommandPool CommandPool;
    };

    struct PushConstantBuffer
    {
        glm::mat4 WorldMatrix;
        VkDeviceAddress VertexBuffer;
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
        void Render(const WindowProperties& props, const DrawContext& drawCommands);
        void Cleanup();

        VmaMeshAsset LoadModel(const std::string& modelPath);

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

        static void SetupSwapchain();
        static vkb::Swapchain CreateSwapchain(uint32_t width, uint32_t height);
        static void DestroySwapchain();
        static void ResizeSwapchain();

        static void CreateMainBuffers(uint32_t width, uint32_t height);
        static void DestroyMainBuffers();
        static void ResizeMainBuffers();
        static void ResizeMainBuffers(uint32_t width, uint32_t height);

        static void SetupCommandBuffers();
        static void SetupSyncStructures();
        static void SetupDescriptors();

        static PipelineWrapper CreatePipeline(const vkb::Device& logicalDevice);

        static void CreateImGuiDescriptorPool();

        inline static void Draw(const WindowProperties& props, const DrawContext& drawCommands);
        inline static void DrawGeometry(VkCommandBuffer cmd, const DrawContext& drawCommands);
        inline static void DrawImGui(VkCommandBuffer cmd, VkImageView targetImageView);

        static void CleanupVulkan();
        static void CleanupImGui();

        static VulkanFrame GetFrame();

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

        inline static Matrix4x4 ViewProjection;

    private:
        inline static SDL_Window* _window;
        inline static VkExtent2D _windowExtent;

        bool _initialized = false;
        inline static bool _rebuildSwapchain;

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
        inline static VkPipelineLayout _pipelineLayout;

        inline static VmaImage _colorImage;
        inline static VmaImage _depthImage;

        // Swapchain
        inline static VkSwapchainKHR _swapchain;
        inline static VkFormat _swapchainImageFormat;
        inline static VkExtent2D _swapchainExtent;
        inline static VkExtent2D _drawExtent;
        inline static std::vector<VkImage> _swapchainImages;
        inline static std::vector<VkImageView> _swapchainImageViews;

        // VMA
        inline static VmaAllocator _vmaAllocator;

        static constexpr int MAX_FRAMES_IN_FLIGHT = 3;
        inline static int _frameIndex = 0;
        inline static VulkanFrame _frameData[MAX_FRAMES_IN_FLIGHT];
        inline static VulkanImmediateBuffer _immBuffer;

        inline static entt::dense_map<std::string, VmaMeshAsset> _loadedModelMap;

        // ImGui
        inline static ImGui_ImplVulkanH_Window _mainWindowData;
        inline static uint32_t _minImageCount = 3;

        inline static VkDescriptorPool _imGuiDescriptorPool = VK_NULL_HANDLE;

        inline static ImGuiIO* _io;
    };
}
