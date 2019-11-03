#include "hl_vulkan.hpp"

namespace HLVulkan {

  bool hasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
           format == VK_FORMAT_D24_UNORM_S8_UINT;
  }

  uint32_t findMemoryType(VkPhysicalDevice phyDevice, uint32_t typeFilter,
                          VkMemoryPropertyFlags properties) {

    // Query physical device for available types of memory
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(phyDevice, &memProperties);

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

} // namespace HLVulkan
