#ifndef __HL_VULKAN_FENCE_HPP__
#define __HL_VULKAN_FENCE_HPP__

#include "device.hpp"
#include "hl_vulkan.hpp"

namespace HLVulkan {

    class Fence {
      private:
        Device device;
        VkFence fence = VK_NULL_HANDLE;

      public:
        Fence(Device device);

        const VkFence getFence();

        ~Fence();
    };

} // namespace HLVulkan

#endif //__HL_VULKAN_FENCE_HPP__