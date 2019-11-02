#include "fence.hpp"

namespace HLVulkan {

  Fence::Fence(VkDevice device) : device(device) {

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    VK_THROW(vkCreateFence(device, &fenceInfo, nullptr, &handle),
             "failed to create fence");
  }

  VkFence Fence::operator*() const { return handle; }

  Fence::~Fence() { vkDestroyFence(device, handle, nullptr); }

} // namespace HLVulkan