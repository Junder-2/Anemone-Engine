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

    struct Vertex
    {
        // Optimal 16-byte alignment layout.
        glm::vec3 Position;
        float UvX;
        glm::vec3 Normal;
        float UvY;
        glm::vec4 Color;
    };

}
