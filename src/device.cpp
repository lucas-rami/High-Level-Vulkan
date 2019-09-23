#include "device.hpp"

namespace HLVulkan {

  Device::Device(VkPhysicalDevice physicalDevice, VkDevice device)
      : physical(physicalDevice), logical(device) {}
  Device::Device(const Device &device)
      : physical(device.physical), logical(device.logical) {}

  uint32_t Device::findMemoryType(uint32_t typeFilter,
                                  VkMemoryPropertyFlags properties) const {

    // Query physical device for available types of memory
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physical, &memProperties);

    // Check each type of memory
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
      if ((typeFilter & (1 << i)) &&
          (memProperties.memoryTypes[i].propertyFlags & properties) ==
              properties) {
        return i;
      }
    }

    // No suitable type found
    return NO_SUITABLE_MEMORY_TYPE;
  }

  VkFormat Device::findSupportedFormat(const std::vector<VkFormat> &candidates,
                                       VkImageTiling tiling,
                                       VkFormatFeatureFlags features) const {

    // Check each candidate format for the necessary features
    for (const auto format : candidates) {
      VkFormatProperties props;
      vkGetPhysicalDeviceFormatProperties(physical, format, &props);

      if (tiling == VK_IMAGE_TILING_LINEAR &&
          (props.linearTilingFeatures & features) == features) {
        return format;
      } else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
                 (props.optimalTilingFeatures & features) == features) {
        return format;
      }
    }

    // No candidate format was found with the necessary features
    ASSERT_MSG(false, "failed to find supported format");
    return VK_FORMAT_UNDEFINED;
  }

  VkFormat Device::findDepthFormat() const {
    return findSupportedFormat({VK_FORMAT_D32_SFLOAT,
                                VK_FORMAT_D32_SFLOAT_S8_UINT,
                                VK_FORMAT_D24_UNORM_S8_UINT},
                               VK_IMAGE_TILING_OPTIMAL,
                               VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
  }

} // namespace HLVulkan
