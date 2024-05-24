#pragma once
#include <entt.hpp>
#include <SDL_video.h>
#include <imgui_impl_vulkan.h>
#include <span>

#include "VmaTypes.h"
#include "VulkanCommon.h"
#include "VulkanDeletionQueue.h"
#include "VulkanDescriptors.h"
#include "VulkanMaterial.h"
#include "VulkanPipelineBuilder.h"
#include "ANE/Math/Types/Matrix4x4.h"
#include "ANE/Renderer/CommonSets.h"
#include "ANE/Renderer/Draw.h"
#include "ANE/Renderer/Mesh.h"

namespace Engine
{
    struct WindowProperties;
}

namespace vkb
{
    struct Instance;
    struct PhysicalDevice;
    struct Device;
    struct Swapchain;
}

namespace Vulkan
{
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
        ApplicationData AppData;
        SceneData SceneData;

        VmaBuffer AppDataBuffer;
        VmaBuffer SceneDataBuffer;
    };

    struct VulkanImmediateBuffer
    {
        VkFence Fence;
        VkCommandBuffer CommandBuffer;
        VkCommandPool CommandPool;
    };

    struct PushConstantBuffer
    {
        Matrix4x4 MVPMatrix;
        Matrix4x4 ModelMatrix;
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

        void OnWindowResize();

        VmaMeshAsset LoadModel(const std::string& modelPath);

        static VmaMeshBuffers UploadRawMesh(std::span<uint32_t> indices, std::span<Vertex> vertices);

        VmaImage LoadTexture(const std::string& texturePath);
        VmaImage LoadCubeTexture(const std::string& texturePath);

        float GetFramerate();
        static ImGuiIO* GetImGuiIO() { return _io; }

        // Engine defaults.
        static VmaMeshAsset GetDefaultMesh() { return _cubeMesh; }
        static MaterialInstance* GetDefaultMaterial() { return &_filamentInstance; }
        MaterialInstance* GetDefaultMaterialClone();

        static VkSampler GetLinearSampler() { return _samplerLinear; }
        static VkSampler GetNearestSampler() { return _samplerNearest; }

        // Vulkan
        static VkDevice GetDevice() { return _device; }
        static VkAllocationCallbacks* GetAllocator() { return _allocator; }
        static std::vector<VkDescriptorSetLayout> GetSceneLayouts() { return { _appDataLayout, _geometryDataLayout}; }
        static VmaImage GetColorBuffer() { return _colorImage; }
        static VmaImage GetDepthBuffer() { return _depthImage; }

        static VkDescriptorSet GetImGuiViewportSet() { return _imGuiViewportSet; }

    private:
        // Function passed for error checking in ImGui.
        static void CheckVkResult(const VkResult err) { CHECK_RESULT(err); }

        static std::vector<const char*> GetAvailableExtensions(SDL_Window* window);

        void SetupVulkan(SDL_Window* window);
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
        static void UpdateImGuiViewportSet();

        static void SetupCommandBuffers();
        static void SetupSyncStructures();
        static void SetupDescriptors();
        static void LoadSlangShader(const char* moduleName, VkShaderModule* vertShader, VkShaderModule* fragShader);

        static void CreatePipeline(const vkb::Device& logicalDevice);

        void CreateDefaultResources();
        void CreateDefaultTextures();
        void CreateDefaultMaterial();

        static void CreateImGuiDescriptorPool();

        inline static void Draw(const WindowProperties& props, const DrawContext& drawCommands);
        inline static void DrawSky(VkCommandBuffer cmd);
        inline static void DrawGeometry(VkCommandBuffer cmd, const DrawContext& drawCommands);
        inline static void DrawDebugGeometry(VkCommandBuffer cmd, const DrawContext& drawCommands);
        inline static void DrawImGui(VkCommandBuffer cmd, VkImageView targetImageView);

        inline static void UpdateGlobalUniforms();

        static void CleanupVulkan();
        static void CleanupImGui();

        static VulkanFrame& GetFrame();

        // VMA
        // TODO: Isolate VMA code to separate file/class.
        static void CreateVmaAllocator();

        static VmaBuffer CreateBuffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);
        static void DestroyBuffer(const VmaBuffer& buffer);

        static VmaImage CreateImage(VkExtent3D size, VkFormat format, VkImageUsageFlags usage, VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D, bool mipmaps = false);
        static VmaImage CreateImage(const void* data, VkExtent3D size, VkFormat format, VkImageUsageFlags usage, VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D, bool mipmaps = false);
        static VmaImage CreateCubeImage(const void* data, VkExtent3D size, VkFormat format, VkImageUsageFlags usage, VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_CUBE, bool mipmaps = false);
        static void DestroyImage(const VmaImage& image);
        static void GenerateMipMaps(const VmaImage& image, VkExtent3D size, uint32_t mipLevels, bool cubemap = false);

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

        inline static Vector3 SunDirection = -Vector3::UpVector();
        inline static Vector3 SunColor = Vector3::ZeroVector();
        inline static Vector3 CameraPosition;
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
        inline static DescriptorAllocator _mainDescriptors;

        inline static VkAllocationCallbacks* _allocator = nullptr;
        inline static VkInstance _instance = VK_NULL_HANDLE;
        inline static VkDebugUtilsMessengerEXT _debugMessenger;
        inline static VkSurfaceKHR _surface;
        inline static VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
        inline static VkDevice _device = VK_NULL_HANDLE;
        inline static QueueFamilyIndices _queueFamily = (QueueFamilyIndices)-1;
        inline static VkQueue _queue = VK_NULL_HANDLE;
        inline static VkPipelineCache _pipelineCache = VK_NULL_HANDLE;
        inline static VkPipeline _skyPipeline = VK_NULL_HANDLE;
        inline static VkPipeline _debugTrianglePipeline = VK_NULL_HANDLE;
        inline static VkPipeline _debugLinePipeline = VK_NULL_HANDLE;
        inline static VkPipelineLayout _pipelineLayout;

        inline static VmaImage _colorImage;
        inline static VmaImage _depthImage;

        inline static VkDescriptorSetLayout _geometryDataLayout;
        inline static VkDescriptorSetLayout _appDataLayout;
        inline static VkDescriptorSetLayout _singleImageDataLayout;

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
        inline static entt::dense_map<std::string, VmaImage> _loadedTextureMap;

        // Engine resources
        inline static VmaImage _whiteImage;
        inline static VmaImage _blackImage;
        inline static VmaImage _greyImage;
        inline static VmaImage _normalImage;
        inline static VmaImage _errorImage;
        inline static VmaImage _blackCubeImage;

        inline static VmaImage _colorTex;
        inline static VmaImage _normalTex;
        inline static VmaImage _ormTex;
        inline static VmaImage _dfgTex;
        inline static VmaImage _cubeMap;

        // Primitives
        inline static VmaMeshAsset _cubeMesh;
        inline static VmaMeshAsset _sphereMesh;
        inline static VmaMeshAsset _skyMesh;

        inline static VkSampler _samplerLinear;
        inline static VkSampler _samplerNearest;

        inline static MaterialInstance _filamentInstance;
        inline static FilamentMetallicRoughness _filamentMaterial;

        inline static std::vector<MaterialInstance*> _materialInstances;

        // ImGui
        inline static ImGui_ImplVulkanH_Window _mainWindowData;
        inline static uint32_t _minImageCount = 3;

        inline static VkDescriptorPool _imGuiDescriptorPool = VK_NULL_HANDLE;
        inline static VkDescriptorSet _imGuiViewportSet;

        inline static ImGuiIO* _io;
    };
}
