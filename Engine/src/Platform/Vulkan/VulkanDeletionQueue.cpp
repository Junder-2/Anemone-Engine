#include "anepch.h"
#include "VulkanDeletionQueue.h"

namespace Engine
{
    void VulkanDeletionQueue::PushFunction(std::function<void()>&& function)
    {
        _deletors.push_back(function);
    }

    void VulkanDeletionQueue::Flush()
    {
        // Vulkan de-allocations have to be done in reverse.
        for (auto it = _deletors.rbegin(); it != _deletors.rend(); it++)
        {
            (*it)(); // Call functors.
        }

        _deletors.clear();
    }
}
