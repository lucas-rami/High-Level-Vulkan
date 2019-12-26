#include "fence.hpp"

namespace HLVulkan {

  Fence::Fence(const Device &device) : device(*device) {

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    VK_THROW(vkCreateFence(this->device, &fenceInfo, nullptr, &fence),
             "failed to create fence");
  }

  VkFence Fence::operator*() const { return fence; }

  Fence::~Fence() {
    if (fence != VK_NULL_HANDLE) {
      vkDestroyFence(device, fence, nullptr);
    }
  }

  Fence::Fence(Fence &&other): device(other.device), fence(other.fence) {
    other.fence = VK_NULL_HANDLE;
  }

  Fence &Fence::operator=(Fence &&other) {
    // Self-assignment detection
    if (&other != this) {
      device = other.device;
      fence = other.fence;

      other.fence = VK_NULL_HANDLE;
    }
    return *this;
  }

} // namespace HLVulkan