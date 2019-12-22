#include "sc_support.hpp"

namespace HLVulkan {

  SCSupport::SCSupport() : capabilities(), formats(), presentModes() {}

  VkResult SCSupport::querySupport(VkPhysicalDevice device,
                                   VkSurfaceKHR surface) {

    // Get surface's capabilities
    VK_RET(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                                     &capabilities));

    // Get supported surface formats
    uint32_t formatCount;
    VK_RET(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                                nullptr));
    if (formatCount != 0) {
      formats.resize(formatCount);
      VK_RET(vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                                  formats.data()));
    }

    // Get supported presentation modes
    uint32_t presentModeCount;
    VK_RET(vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface, &presentModeCount, nullptr));
    if (presentModeCount != 0) {
      presentModes.resize(presentModeCount);
      VK_RET(vkGetPhysicalDeviceSurfacePresentModesKHR(
          device, surface, &presentModeCount, presentModes.data()));
    }

    return VK_SUCCESS;
  }

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

} // namespace HLVulkan