#ifndef __HL_VULKAN_SWAPCHAIN_HPP__
#define __HL_VULKAN_SWAPCHAIN_HPP__

#include "device.hpp"

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
  };

  class SwapChain {

  public:
    SwapChain(const Device &device, const Surface &surface);

    static VkResult querySupport(VkPhysicalDevice device, VkSurfaceKHR surface,
                                 SCSupport &support);

  private:
    // The swapchain handle
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;
    // Surface format
    VkSurfaceFormatKHR surfaceFormat;
    // Presentation mode
    VkPresentModeKHR presentMode;
    // Swapchain extent
    VkExtent2D extent;
    // Owned images
    std::vector<VkImage> images;
    // Owned image views
    std::vector<VkImageView> views;
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_SWAPCHAIN_HPP__