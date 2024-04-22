#include "anepch.h"
#include "VulkanDescriptors.h"

namespace Engine
{
    DescriptorLayoutBuilder::DescriptorLayoutBuilder(vkb::Device const& logicalDevice)
    {
        _info.LogicalDevice = logicalDevice;
    }

    DescriptorLayoutBuilder& DescriptorLayoutBuilder::SetStageFlags(const VkShaderStageFlags flags)
    {
        _info.StageFlags = flags;

        return *this;
    }

    DescriptorLayoutBuilder& DescriptorLayoutBuilder::SetCreateFlags(const VkDescriptorSetLayoutCreateFlags flags)
    {
        _info.CreateFlags = flags;

        return *this;
    }

    DescriptorLayoutBuilder& DescriptorLayoutBuilder::AddBinding(const uint32_t binding, const VkDescriptorType type)
    {
        VkDescriptorSetLayoutBinding newBind = { };
        newBind.binding = binding;
        newBind.descriptorType = type;
        newBind.descriptorCount = 1;

        _info.Bindings.push_back(newBind);

        return *this;
    }

    DescriptorLayoutBuilder& DescriptorLayoutBuilder::ClearBindings()
    {
        _info.Bindings.clear();

        return *this;
    }

    DescriptorLayoutBuilder& DescriptorLayoutBuilder::SetAllocationCallbacks(VkAllocationCallbacks* callbacks)
    {
        _info.AllocationCallbacks = callbacks;

        return *this;
    }

    VkDescriptorSetLayout DescriptorLayoutBuilder::Build()
    {
        for (VkDescriptorSetLayoutBinding& binding : _info.Bindings)
        {
            binding.stageFlags = _info.StageFlags;
        }

        VkDescriptorSetLayoutCreateInfo layoutInfo = { .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO, .pNext = nullptr };
        layoutInfo.flags = _info.CreateFlags;
        layoutInfo.bindingCount = (uint32_t)_info.Bindings.size();
        layoutInfo.pBindings = _info.Bindings.data();

        VkDescriptorSetLayout layout;
        const VkResult result = vkCreateDescriptorSetLayout(_info.LogicalDevice, &layoutInfo, _info.AllocationCallbacks, &layout);
        ANE_EASSERT(result == VK_SUCCESS, "Failed to create VkDescriptorSetLayout.");

        return layout;
    }

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

    VkDescriptorSet DescriptorAllocator::Allocate(const VkDevice logicalDevice, const VkDescriptorSetLayout layout, const VkAllocationCallbacks* callbacks)
    {
        VkDescriptorPool pool = GetPool(logicalDevice, callbacks);

        VkDescriptorSetAllocateInfo allocInfo = { .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, .pNext = nullptr };
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
}
