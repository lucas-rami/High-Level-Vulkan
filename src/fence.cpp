#include "fence.hpp"

namespace HLVulkan {

    Fence::Fence(Device device) : device(device) {

        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        VK_CHECK_FAIL(vkCreateFence(device.logical, &fenceInfo, nullptr, &fence), "failed to create fence");
    }

    const VkFence Fence::getFence() { return fence; }

    Fence::~Fence() { vkDestroyFence(device.logical, fence, nullptr); }

} // namespace HLVulkan