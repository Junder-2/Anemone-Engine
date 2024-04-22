#pragma once
#include <span>
#include <vulkan/vulkan_core.h>

#include "VkBootstrap.h"

namespace Engine
{
    class DescriptorLayoutBuilder
    {
    public:
        explicit DescriptorLayoutBuilder(vkb::Device const& logicalDevice);

        DescriptorLayoutBuilder& SetStageFlags(VkShaderStageFlags flags);

        DescriptorLayoutBuilder& SetCreateFlags(VkDescriptorSetLayoutCreateFlags flags);

        DescriptorLayoutBuilder& AddBinding(uint32_t binding, VkDescriptorType type);

        DescriptorLayoutBuilder& ClearBindings();

        DescriptorLayoutBuilder& SetAllocationCallbacks(VkAllocationCallbacks* callbacks);

        VkDescriptorSetLayout Build();

    private:
        struct LayoutInfo
        {
            VkDevice LogicalDevice = VK_NULL_HANDLE;

            VkShaderStageFlags StageFlags;
            std::vector<VkDescriptorSetLayoutBinding> Bindings;

            VkDescriptorSetLayoutCreateFlags CreateFlags = 0;

            VkAllocationCallbacks* AllocationCallbacks = VK_NULL_HANDLE;
        } _info;
    };

    class DescriptorAllocator
    {
    public:
        struct PoolSizeRatio
        {
            VkDescriptorType Type;
            float Ratio;
        };

        void Init(VkDevice logicalDevice, uint32_t maxSets, std::span<PoolSizeRatio> poolRatios, const VkAllocationCallbacks* callbacks);

        void Clear(VkDevice logicalDevice);

        void Destroy(VkDevice logicalDevice, const VkAllocationCallbacks* callbacks);

        VkDescriptorSet Allocate(VkDevice logicalDevice, VkDescriptorSetLayout layout, const VkAllocationCallbacks* callbacks);

    private:

        VkDescriptorPool GetPool(VkDevice logicalDevice, const VkAllocationCallbacks* callbacks);

        VkDescriptorPool CreatePool(VkDevice logicalDevice, uint32_t setCount, std::span<PoolSizeRatio> poolRatios, const VkAllocationCallbacks* callbacks);

        std::vector<PoolSizeRatio> _ratios;
        std::vector<VkDescriptorPool> _fullPools;
        std::vector<VkDescriptorPool> _readyPools;
        uint32_t _setsPerPool;
    };
}
