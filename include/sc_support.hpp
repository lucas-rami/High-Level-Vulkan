#ifndef __HL_VULKAN_SC_SUPPORT_HPP__
#define __HL_VULKAN_SC_SUPPORT_HPP__

#include "hl_vulkan.hpp"

namespace HLVulkan {

  /**
   * @brief Aggregates all information about the swapchain support for a
   * physical device.
   */
  struct SCSupport {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

    /**
     * @brief Creates an empty swapchain support. Typically, an empty swapchain
     * support would be passed to the SwapChain.querySupport() method.
     */
    SCSupport();

    /**
     * @brief Copy-constructor.
     */
    SCSupport(const SCSupport &other);

    /**
     * @brief Copy-assignment operator.
     */
    SCSupport &operator=(const SCSupport &other);

    /**
     * @brief Move-constructor.
     */
    SCSupport(const SCSupport &&other);

    /**
     * @brief Move-assignment operator.
     */
    SCSupport &operator=(const SCSupport &&other);

    // @TODO doc
    VkResult querySupport(VkPhysicalDevice device, VkSurfaceKHR surface);
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_SC_SUPPORT_HPP__