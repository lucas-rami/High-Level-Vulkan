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