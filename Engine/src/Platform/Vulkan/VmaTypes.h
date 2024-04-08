#pragma once

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace Engine
{
    struct VmaImage
    {
        VkImage Image;
        VkImageView ImageView;
        VkExtent3D ImageExtent;
        VkFormat ImageFormat;
        VmaAllocation Allocation;
    };

    struct VmaBuffer
    {
        VkBuffer Buffer;
        VmaAllocation Allocation;
        VmaAllocationInfo Info;
    };

    struct VmaMeshBuffers
    {
        VmaBuffer IndexBuffer;
        VmaBuffer VertexBuffer;
        VkDeviceAddress VertexBufferAddress;
    };
}
