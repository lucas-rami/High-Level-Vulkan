#ifndef __HL_VULKAN_DEVICE_HPP__
#define __HL_VULKAN_DEVICE_HPP__

#include "hl_vulkan.hpp"

#define NO_SUITABLE_MEMORY_TYPE 255

namespace HLVulkan {

  /**
   * @brief Packages a VkPhysicalDevice together with a VkDevice.
   */
  struct Device {
    const VkPhysicalDevice physical;
    const VkDevice logical;

    /**
     * @brief Creates a Device instance from a VkPhysicalDevice and a VkDevice.
     * The logical device must have been created from the physical one.
     *
     * @param physicalDevice A physical device.
     * @param device A logical device.
     */
    Device(VkPhysicalDevice physicalDevice, VkDevice device);

    /**
     * @brief Straightforward copy constructor (shallow copies all fields).
     *
     * @param device The device to copy.
     */
    Device(const Device &device);

    /**
     * @brief Finds a suitable memory type on the device given a bitmask of
     * supported memory types and a set of desired properties.
     *
     * @param typeFilter A bitmask of supported memory types on the device. Must
     * be the memoryTypeBits field of a VkMemoryRequirements struct.
     * @param properties A set of desired properties.
     *
     * @return A suitable memory type of minimum index if it exists,
     * NO_SUITABLE_MEMORY_TYPE otherwise.
     */
    uint32_t findMemoryType(uint32_t typeFilter,
                            VkMemoryPropertyFlags properties) const;

    /**
     * @brief Finds a supported format on the device among an ordered list of
     * candidates.
     *
     * @param candidates An ordered list of candidate formats.
     * @param tiling The tiling to use along the format. One of
     * VK_IMAGE_TILING_LINEAR or VK_IMAGE_TILING_OPTIMAL.
     * @param features A set of desired features for the format.
     *
     * @return The first format among the candidates which supports the desired
     * features for the requested tiling mode. VK_FORMAT_UNDEFINED if no format
     * is supported.
     */
    VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
                                 VkImageTiling tiling,
                                 VkFormatFeatureFlags features) const;

    /**
     * @brief Finds a supported depth-stencil format on the device. Simply calls
     * findSupportedFormat() with appropriate arguments for a depth-stencil
     * format.
     *
     * @return See findSupportedFormat().
     */
    VkFormat findDepthFormat() const;
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_DEVICE_HPP__