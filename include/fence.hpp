#ifndef __HL_VULKAN_FENCE_HPP__
#define __HL_VULKAN_FENCE_HPP__

#include "device.hpp"

namespace HLVulkan {

  class Fence {

  public:
    Fence(VkDevice device);

    VkFence operator*() const;

    virtual ~Fence();

  private:
    VkDevice device = VK_NULL_HANDLE;
    VkFence handle = VK_NULL_HANDLE;
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_FENCE_HPP__