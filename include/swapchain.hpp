#ifndef __HL_VULKAN_SWAPCHAIN_HPP__
#define __HL_VULKAN_SWAPCHAIN_HPP__

#include "device.hpp"

namespace HLVulkan {

  class SwapChain {

  public:
    SwapChain(const Surface &surface, const Device &device);

  private:
    // The swapchain handle
    VkSwapchainKHR swapchain{VK_NULL_HANDLE};
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