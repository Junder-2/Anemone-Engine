#pragma once

#include <deque>

namespace Vulkan
{
    struct VulkanDeletionQueue
    {
        void PushFunction(std::function<void()>&& function);

        void Flush();

    private:
        // TODO: Store destroy call arguments instead of entire functions.
        std::deque<std::function<void()>> _deletors;
    };
}
