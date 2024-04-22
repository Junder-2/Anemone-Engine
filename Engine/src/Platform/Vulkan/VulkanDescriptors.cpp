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
}
