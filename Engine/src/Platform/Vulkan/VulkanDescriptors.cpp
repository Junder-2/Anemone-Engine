#include "anepch.h"
#include "VulkanDescriptors.h"

namespace Vulkan
{
    void DescriptorAllocator::Init(const VkDevice logicalDevice, const uint32_t maxSets, const std::span<PoolSizeRatio> poolRatios, const VkAllocationCallbacks* callbacks)
    {
        _ratios.clear();
        for (const PoolSizeRatio ratio : poolRatios)
        {
            _ratios.push_back(ratio);
        }

        const VkDescriptorPool newPool = CreatePool(logicalDevice, maxSets, poolRatios, callbacks);

        _setsPerPool = (uint32_t)((float)maxSets * 1.5f);

        _readyPools.push_back(newPool);
    }

    void DescriptorAllocator::Clear(const VkDevice logicalDevice)
    {
        for (const VkDescriptorPool pool : _readyPools)
        {
            vkResetDescriptorPool(logicalDevice, pool, 0);
        }

        for (VkDescriptorPool pool : _fullPools)
        {
            vkResetDescriptorPool(logicalDevice, pool, 0);
            _readyPools.push_back(pool);
        }

        _fullPools.clear();
    }

    void DescriptorAllocator::Destroy(const VkDevice logicalDevice, const VkAllocationCallbacks* callbacks)
    {
        for (const VkDescriptorPool pool : _readyPools)
        {
            vkDestroyDescriptorPool(logicalDevice, pool, callbacks);
        }

        _readyPools.clear();

        for (const VkDescriptorPool pool : _fullPools)
        {
            vkDestroyDescriptorPool(logicalDevice, pool, callbacks);
        }

        _fullPools.clear();
    }

    VkDescriptorSet DescriptorAllocator::Allocate(const VkDevice logicalDevice, const VkDescriptorSetLayout layout, const VkAllocationCallbacks* callbacks, void* pNext)
    {
        VkDescriptorPool pool = GetPool(logicalDevice, callbacks);

        VkDescriptorSetAllocateInfo allocInfo = { .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
        allocInfo.pNext = pNext;

        allocInfo.descriptorPool = pool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &layout;

        VkDescriptorSet set;
        VkResult result = vkAllocateDescriptorSets(logicalDevice, &allocInfo, &set);
        if (result == VK_ERROR_OUT_OF_POOL_MEMORY || result == VK_ERROR_FRAGMENTED_POOL)
        {
            _fullPools.push_back(pool);

            pool = GetPool(logicalDevice, callbacks);
            allocInfo.descriptorPool = pool;

            result = vkAllocateDescriptorSets(logicalDevice, &allocInfo, &set);
            ANE_EASSERT(result == VK_SUCCESS, "Failed to allocate VkDescriptorSets.");
        }

        _readyPools.push_back(pool);

        return set;
    }

    VkDescriptorPool DescriptorAllocator::GetPool(const VkDevice logicalDevice, const VkAllocationCallbacks* callbacks)
    {
        VkDescriptorPool newPool;
        if (_readyPools.empty())
        {
            newPool = CreatePool(logicalDevice, _setsPerPool, _ratios, callbacks);

            _setsPerPool = (uint32_t)((float)_setsPerPool * 1.5f);
            if (_setsPerPool > 4092)
            {
                _setsPerPool = 4092;
            }
        }
        else
        {
            newPool = _readyPools.back();
            _readyPools.pop_back();
        }

        return newPool;
    }

    VkDescriptorPool DescriptorAllocator::CreatePool(const VkDevice logicalDevice, const uint32_t setCount, const std::span<PoolSizeRatio> poolRatios, const VkAllocationCallbacks* callbacks)
    {
        std::vector<VkDescriptorPoolSize> poolSizes;
        for (const PoolSizeRatio ratio : poolRatios)
        {
            poolSizes.push_back(VkDescriptorPoolSize
            {
                .type = ratio.Type,
                .descriptorCount = (uint32_t)(ratio.Ratio * (float)setCount)
            });
        }

        VkDescriptorPoolCreateInfo poolInfo = { .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO, .pNext = nullptr };
        poolInfo.flags = 0;
        poolInfo.maxSets = setCount;
        poolInfo.poolSizeCount = (uint32_t)poolSizes.size();
        poolInfo.pPoolSizes = poolSizes.data();

        VkDescriptorPool newPool;
        const VkResult result = vkCreateDescriptorPool(logicalDevice, &poolInfo, callbacks, &newPool);
        ANE_EASSERT(result == VK_SUCCESS, "Failed to create VkDescriptorPool.");

        return newPool;
    }

    void DescriptorWriter::WriteBuffer(const int binding, const VkBuffer buffer, const size_t size, const size_t offset, const VkDescriptorType type)
    {
        const VkDescriptorBufferInfo& info = BufferInfos.emplace_back(VkDescriptorBufferInfo
        {
            .buffer = buffer,
            .offset = offset,
            .range = size
        });

        VkWriteDescriptorSet write = { .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, .pNext = nullptr };
        write.dstSet = VK_NULL_HANDLE; // Empty for now.
        write.dstBinding = binding;
        write.descriptorCount = 1;
        write.descriptorType = type;
        write.pBufferInfo = &info;

        Writes.push_back(write);
    }

    void DescriptorWriter::WriteImage(const int binding, const VkImageView image, const VkSampler sampler, const VkImageLayout layout, const VkDescriptorType type)
    {
        const VkDescriptorImageInfo& info = ImageInfos.emplace_back(VkDescriptorImageInfo
        {
            .sampler = sampler,
            .imageView = image,
            .imageLayout = layout
        });

        VkWriteDescriptorSet write = { .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET, .pNext = nullptr };
        write.dstSet = VK_NULL_HANDLE; // Empty for now.
        write.dstBinding = binding;
        write.descriptorCount = 1;
        write.descriptorType = type;
        write.pImageInfo = &info;

        Writes.push_back(write);
    }

    void DescriptorWriter::Clear()
    {
        ImageInfos.clear();
        BufferInfos.clear();
        Writes.clear();
    }

    void DescriptorWriter::UpdateSet(const VkDevice logicalDevice, const VkDescriptorSet set)
    {
        for (VkWriteDescriptorSet& write : Writes)
        {
            write.dstSet = set;
        }

        vkUpdateDescriptorSets(logicalDevice, (uint32_t)Writes.size(), Writes.data(), 0, nullptr);
    }
}
