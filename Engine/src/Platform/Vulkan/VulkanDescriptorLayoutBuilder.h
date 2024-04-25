#pragma once
#include <vulkan/vulkan_core.h>

namespace Engine
{
    class DescriptorLayoutBuilder
    {
    public:
        explicit DescriptorLayoutBuilder(VkDevice const& logicalDevice);

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
}
