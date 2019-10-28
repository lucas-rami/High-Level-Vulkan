#ifndef __HL_VULKAN_QUEUE_HPP__
#define __HL_VULKAN_QUEUE_HPP__

#include "hl_vulkan.hpp"

#include <optional>

namespace HLVulkan {

  /**
   * @brief TODO
   */
  struct Queue {
    VkFlags flags;
    bool present;
    uint32_t family = 0;
    VkQueue handle = VK_NULL_HANDLE;

    Queue(VkFlags flags, bool present = false);

    Queue(VkFlags flags, bool present, uint32_t family, VkQueue handle);

    static VkResult checkSupport(VkPhysicalDevice device, VkSurfaceKHR surface,
                                 std::vector<Queue> &queues, bool &supportOK);
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_QUEUE_HPP__