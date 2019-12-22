#ifndef __HL_VULKAN_SWAPCHAIN_HPP__
#define __HL_VULKAN_SWAPCHAIN_HPP__

#include "device.hpp"

namespace HLVulkan {

  /**
   * @brief Packages a VkSwapchainKHR.
   */
  class SwapChain {

  public:
    /**
     * @brief Creates a swapchain.
     *
     * @param[in] surface The surface the swapchain will render to.
     * @param[in] device The device to create the swapchain from.
     *
     * @throw std::runtime_error If the device doesn't have queues supporting
     * graphics and presentation operations. If swapchain creation fails for
     * some reason.
     */
    SwapChain(const Surface &surface, const Device &device);

    /**
     * @brief Deleted copy-constructor.
     */
    SwapChain(SwapChain &other) = delete;

    /**
     * @brief Deleted copy-assignment operator.
     */
    SwapChain &operator=(SwapChain &other) = delete;

    /**
     * @brief Move-constructor.
     */
    SwapChain(SwapChain &&other);

    /**
     * @brief Move-assignment operator.
     */
    SwapChain &operator=(SwapChain &&other);

    virtual ~SwapChain();

  private:
    // The associated device
    VkDevice device{VK_NULL_HANDLE};
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