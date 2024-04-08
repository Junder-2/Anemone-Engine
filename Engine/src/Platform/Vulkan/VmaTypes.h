#pragma once

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace Engine
{
    struct VmaImage
    {
        VkImage image;
        VkImageView imageView;
        VmaAllocation allocation;
        VkExtent3D imageExtent;
        VkFormat imageFormat;
    };
}
