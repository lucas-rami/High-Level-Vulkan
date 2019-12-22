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
     * @brief Queries the swapchain support for a physical device and surface.
     * When the method returns with success, the receiver object is filled with
     * the capabilities, formats, and presentation modes supported for the
     * device and surface. 
     * 
     * @param[in] device A physical device.
     * @param[in] surface A surface.
     * 
     * @return A Vulkan error code if a call to the Vulkan API failed,
     * VK_SUCCESS otherwise.
     */
    VkResult querySupport(VkPhysicalDevice device, VkSurfaceKHR surface);

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
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_SC_SUPPORT_HPP__