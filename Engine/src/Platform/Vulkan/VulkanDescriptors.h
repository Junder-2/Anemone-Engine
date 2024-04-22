#pragma once
#include <deque>
#include <span>
#include <vulkan/vulkan_core.h>

namespace Engine
{
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
        uint32_t _setsPerPool = 0;
    };

    class DescriptorWriter
    {
    public:
        void WriteBuffer(int binding, VkBuffer buffer, size_t size, size_t offset, VkDescriptorType type);

        void WriteImage(int binding, VkImageView image, VkSampler sampler, VkImageLayout layout, VkDescriptorType type);

        void Clear();

        void UpdateSet(VkDevice logicalDevice, VkDescriptorSet set);

        std::deque<VkDescriptorBufferInfo> BufferInfos;
        std::deque<VkDescriptorImageInfo> ImageInfos;
        std::vector<VkWriteDescriptorSet> Writes;
    };
}
