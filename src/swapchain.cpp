#include "swapchain.hpp"
#include "hl_vulkan.hpp"

namespace HLVulkan {

  // ********** SCSupport **********

  SCSupport::SCSupport() : capabilities(), formats(), presentModes() {}

  SCSupport::SCSupport(const SCSupport &other)
      : capabilities(other.capabilities), formats(other.formats),
        presentModes(other.presentModes) {}

  SCSupport &SCSupport::operator=(const SCSupport &other) {
    // Self-assignment detection
    if (&other != this) {
      capabilities = other.capabilities;
      formats = other.formats;
      presentModes = other.presentModes;
    }
    return *this;
  }

  SCSupport::SCSupport(const SCSupport &&other)
      : capabilities(capabilities), formats(std::move(other.formats)),
        presentModes(std::move(other.presentModes)) {}

  SCSupport &SCSupport::operator=(const SCSupport &&other) {
    // Self-assignment detection
    if (&other != this) {
      capabilities = other.capabilities;
      formats = std::move(other.formats);
      presentModes = std::move(other.presentModes);
    }
    return *this;
  }

  // ********** SwapChain **********

  VkSurfaceFormatKHR
  chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &candidates) {

    // Surface has no preferred format (we are free to choose)
    if (candidates.size() == 1 && candidates[0].format == VK_FORMAT_UNDEFINED) {
      return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    // Select preferred format if it is available
    for (const auto &format : candidates) {
      if (format.format == VK_FORMAT_B8G8R8A8_UNORM &&
          format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        return format;
      }
    }

    // Otherwise take first proposed format
    return candidates[0];
  }

  VkPresentModeKHR
  choosePresentMode(const std::vector<VkPresentModeKHR> candidates) {
    // Vertical sync (double buffering)
    auto bestMode = VK_PRESENT_MODE_FIFO_KHR;

    for (const auto &availablePresentMode : candidates) {
      if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
        // Triple buffering
        return availablePresentMode;
      } else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
        // No buffering
        bestMode = availablePresentMode;
      }
    }

    // VK_PRESENT_MODE_MAILBOX_KHR > VK_PRESENT_MODE_IMMEDIATE_KHR >
    // VK_PRESENT_MODE_FIFO_KHR
    return bestMode;
  }

  VkExtent2D chooseExtent(const VkSurfaceCapabilitiesKHR &capabilities) {
    if (capabilities.currentExtent.width !=
        std::numeric_limits<uint32_t>::max()) {
      return capabilities.currentExtent;
    } else {
      int width, height;
      glfwGetFramebufferSize(window, &width, &height);

      VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                                 static_cast<uint32_t>(height)};

      actualExtent.width = std::max(
          capabilities.minImageExtent.width,
          std::min(capabilities.maxImageExtent.width, actualExtent.width));
      actualExtent.height = std::max(
          capabilities.minImageExtent.height,
          std::min(capabilities.maxImageExtent.height, actualExtent.height));

      return actualExtent;
    }
  }

  SwapChain::SwapChain(const Device &device, const Surface &surface) {

    // Choose swapchain characteristics
    SCSupport deviceSupport = device.getSwapchainSupport();
    surfaceFormat = chooseSurfaceFormat(deviceSupport.formats);
    presentMode = choosePresentMode(deviceSupport.presentModes);
    extent = chooseExtent(deviceSupport.capabilities);
    
  }

  VkResult SwapChain::querySupport(VkPhysicalDevice device,
                                   VkSurfaceKHR surface, SCSupport &support) {

    // Get surface's capabilities
    VK_RET(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                                     &support.capabilities));

    // Get supported surface formats
    uint32_t formatCount;
    VK_RET(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                                nullptr));
    if (formatCount != 0) {
      support.formats.resize(formatCount);
      VK_RET(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                                  support.formats.data()));
    }

    // Get supported presentation modes
    uint32_t presentModeCount;
    VK_RET(vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface, &presentModeCount, nullptr));
    if (presentModeCount != 0) {
      support.presentModes.resize(presentModeCount);
      VK_RET(vkGetPhysicalDeviceSurfacePresentModesKHR(
          device, surface, &presentModeCount, support.presentModes.data()));
    }

    return VK_SUCCESS;
  }

} // namespace HLVulkan