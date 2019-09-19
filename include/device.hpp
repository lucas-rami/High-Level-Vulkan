#ifndef __HL_VULKAN_DEVICE_HPP__
#define __HL_VULKAN_DEVICE_HPP__

#include "hl_vulkan.hpp"

namespace HLVulkan {

    struct Device {
        const VkPhysicalDevice physical;
        const VkDevice logical;

        Device(VkPhysicalDevice physicalDevice, VkDevice device);
        Device(const Device &device);

        std::optional<uint32_t> findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;

        VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

        VkFormat findDepthFormat() const;
    };

} // namespace HLVulkan

#endif //__HL_VULKAN_DEVICE_HPP__