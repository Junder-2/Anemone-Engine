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

    void DescriptorAllocator::Init(const VkDevice logicalDevice, const uint32_t maxSets, const std::span<PoolSizeRatio> poolRatios, const VkAllocationCallbacks* callbacks = nullptr)
    {
        std::vector<VkDescriptorPoolSize> poolSizes;
        for (const PoolSizeRatio ratio : poolRatios)
        {
            poolSizes.push_back(VkDescriptorPoolSize
            {
                .type = ratio.Type,
                .descriptorCount = (uint32_t)(ratio.Ratio * (float)maxSets)
            });
        }

        VkDescriptorPoolCreateInfo poolInfo = { .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
        poolInfo.flags = 0;
        poolInfo.maxSets = maxSets;
        poolInfo.poolSizeCount = (uint32_t)poolSizes.size();
        poolInfo.pPoolSizes = poolSizes.data();

        vkCreateDescriptorPool(logicalDevice, &poolInfo, callbacks, &Pool);
    }

    void DescriptorAllocator::Clear(const VkDevice logicalDevice)
    {
        vkResetDescriptorPool(logicalDevice, Pool, 0);
    }

    void DescriptorAllocator::Destroy(const VkDevice logicalDevice, const VkAllocationCallbacks* callbacks = nullptr)
    {
        vkDestroyDescriptorPool(logicalDevice, Pool, callbacks);
    }

    VkDescriptorSet DescriptorAllocator::Allocate(const VkDevice logicalDevice, const VkDescriptorSetLayout layout)
    {
        VkDescriptorSetAllocateInfo allocInfo = { .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO, .pNext = nullptr };
        allocInfo.descriptorPool = Pool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &layout;

        VkDescriptorSet set;
        const VkResult result = vkAllocateDescriptorSets(logicalDevice, &allocInfo, &set);
        ANE_EASSERT(result == VK_SUCCESS, "Failed to allocate VkDescriptorSet.");

        return set;
    }
}
