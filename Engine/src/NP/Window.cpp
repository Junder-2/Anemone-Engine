#include "nppch.h"
#include "Window.h"

#include <SDL.h>
#include <SDL_vulkan.h>
#include <vulkan/vulkan.h>

namespace Engine
{
    Window::Window(const WindowProperties& props)
    {
        _windowData = props;
        Init(props);
    }

    Window::~Window()
    {
        Shutdown();
    }

    void Window::Init(const WindowProperties& props)
    {
        NP_ENGINE_LOG_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
        {
            NP_ENGINE_LOG_ERROR("Could not init sdl");
        }

        SDL_Vulkan_LoadLibrary(nullptr);
        _windowContext = SDL_CreateWindow(props.Title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _windowData.Width, _windowData.Height, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);

        uint32_t extensionCount;
        const char** extensionNames = 0;
        SDL_Vulkan_GetInstanceExtensions(_windowContext, &extensionCount, nullptr);
        extensionNames = new const char *[extensionCount];
        SDL_Vulkan_GetInstanceExtensions(_windowContext, &extensionCount, extensionNames);
        const VkInstanceCreateInfo instInfo = {
            VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO, // sType
            nullptr,                                // pNext
            0,                                      // flags
            nullptr,                                // pApplicationInfo
            0,                                      // enabledLayerCount
            nullptr,                                // ppEnabledLayerNames
            extensionCount,                         // enabledExtensionCount
            extensionNames,                         // ppEnabledExtensionNames
        };

        vkCreateInstance(&instInfo, nullptr, &_vkInstance);

        uint32_t physicalDeviceCount;
        vkEnumeratePhysicalDevices(_vkInstance, &physicalDeviceCount, nullptr);
        std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
        vkEnumeratePhysicalDevices(_vkInstance, &physicalDeviceCount, physicalDevices.data());
        VkPhysicalDevice physicalDevice = physicalDevices[0];

        uint32_t queueFamilyCount;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        VkSurfaceKHR surface;
        SDL_Vulkan_CreateSurface(_windowContext, _vkInstance, &surface);

        uint32_t graphicsQueueIndex = UINT32_MAX;
        uint32_t presentQueueIndex = UINT32_MAX;
        VkBool32 support;
        uint32_t i = 0;
        for (VkQueueFamilyProperties queueFamily : queueFamilies) {
            if (graphicsQueueIndex == UINT32_MAX && queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                graphicsQueueIndex = i;
            if (presentQueueIndex == UINT32_MAX) {
                vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &support);
                if(support)
                    presentQueueIndex = i;
            }
            ++i;
        }

        float queuePriority = 1.0f;
        VkDeviceQueueCreateInfo queueInfo = {
            VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, // sType
            nullptr,                                    // pNext
            0,                                          // flags
            graphicsQueueIndex,                         // graphicsQueueIndex
            1,                                          // queueCount
            &queuePriority,                             // pQueuePriorities
        };

        VkPhysicalDeviceFeatures deviceFeatures = {};
        const char* deviceExtensionNames[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        VkDeviceCreateInfo createInfo = {
            VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,   // sType
            nullptr,                                // pNext
            0,                                      // flags
            1,                                      // queueCreateInfoCount
            &queueInfo,                             // pQueueCreateInfos
            0,                                      // enabledLayerCount
            nullptr,                                // ppEnabledLayerNames
            1,                                      // enabledExtensionCount
            deviceExtensionNames,                   // ppEnabledExtensionNames
            &deviceFeatures,                        // pEnabledFeatures
        };
        vkCreateDevice(physicalDevice, &createInfo, nullptr, &_vkDevice);

        VkQueue graphicsQueue;
        vkGetDeviceQueue(_vkDevice, graphicsQueueIndex, 0, &graphicsQueue);

        VkQueue presentQueue;
        vkGetDeviceQueue(_vkDevice, presentQueueIndex, 0, &presentQueue);
    }

    void Window::OnUpdate()
    {
        SDL_Event windowEvent;
        while(SDL_PollEvent(&windowEvent))
        {
            switch (windowEvent.type)
            {
            case SDL_QUIT:
                if(WindowCloseDelegate) WindowCloseDelegate();
                break;
            case SDL_WINDOWEVENT:
                int newWidth, newHeight;
                SDL_GetWindowSize(_windowContext, &newWidth, &newHeight);
                if(newWidth != _windowData.Width || newHeight != _windowData.Height)
                {
                    _windowData.Width = newWidth;
                    _windowData.Height = newHeight;

                    if(WindowResizeDelegate) WindowResizeDelegate(newWidth, newHeight);
                }
            }
        }
    }

    void Window::Shutdown()
    {
        vkDestroyDevice(_vkDevice, nullptr);
        vkDestroyInstance(_vkInstance, nullptr);
        SDL_Vulkan_UnloadLibrary();
        SDL_DestroyWindow(_windowContext);
        SDL_Quit();
    }

    void Window::SetVSync(const bool enabled)
    {
        //todo
        _windowData.VSync = enabled;
    }

    std::unique_ptr<Window> Window::Create(const WindowProperties& props)
    {
        return std::make_unique<Window>(props);
    }
}


