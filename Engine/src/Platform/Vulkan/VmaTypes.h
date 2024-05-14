#pragma once

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

#include "ANE/Math/Types/Vector3.h"
#include "ANE/Math/Types/Vector4.h"

namespace Vulkan
{
    using namespace Engine;

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
        Vector3 Position;
        float UvX;
        Vector3 Normal;
        float UvY;
        Vector4 Color;
        Vector4 Tangent;
    };

}
