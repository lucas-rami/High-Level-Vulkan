#ifndef __HL_VULKAN_DEVICE_HPP__
#define __HL_VULKAN_DEVICE_HPP__

#include "hl_vulkan.hpp"
#include "instance.hpp"
#include "queue.hpp"
#include "surface.hpp"
#include "sc_support.hpp"

// Error code indicating that no suitable memory type could be found in the
// device.
#define NO_SUITABLE_MEMORY_TYPE 255

namespace HLVulkan {

  /**
   * @brief Represents a Vulkan-usable logical device. It also holds queue
   * handles for the device.
   */
  class Device {
  public:
    /**
     * @brief Creates a logical device to be used by Vulkan.
     *
     * The created device is compliant with the passed QueueRequest object, i.e.
     * its retrieved queues have the capabilities defined in the request.
     *
     * @param[in] surface The surface to render to (used to query for swapchain
     * and queue support).
     * @param[in] reqQueues Defines which kind of queues we wish to retrieve
     * from the device.
     *
     * @throw std::runtime_error If for some reason a suitable device could not
     * be created.
     */
    Device(const Surface &surface, const std::vector<Queue> &reqQueues);

    /**
     * @brief Deleted copy-constructor.
     */
    Device(Device &other) = delete;

    /**
     * @brief Deleted copy-assignment operator.
     */
    Device &operator=(Device &other) = delete;

    /**
     * @brief Move-constructor.
     */
    Device(Device &&other);

    /**
     * @brief Move-assignment operator.
     */
    Device &operator=(Device &&other);

    // @TODO doc
    SCSupport getSwapchainSupport() const;

    // @TODO doc
    std::optional<uint32_t> getQueueFamily(VkFlags flags) const;

    // @TODO doc
    std::optional<uint32_t> getPresentQueueFamily() const;

    /**
     * @brief Finds a suitable memory type on the device given a bitmask of
     * supported memory types and a set of desired properties.
     *
     * @param[in] typeFilter A bitmask of supported memory types on the device.
     * Must be the memoryTypeBits field of a VkMemoryRequirements struct.
     * @param[in] properties A set of desired properties.
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
     * @param[in] candidates An ordered list of candidate formats.
     * @param[in] tiling The tiling to use along the format. One of
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

    /**
     * @brief Returns the packaged VkDevice.
     */
    VkDevice operator*() const;

    ~Device();

  private:
    // The physical device
    VkPhysicalDevice physical = VK_NULL_HANDLE;
    // The logical device
    VkDevice logical = VK_NULL_HANDLE;
    // Defines the swapchain support for the device
    SCSupport scSupport;
    // Queues instantiated from the device 
    std::vector<Queue> queues;
  };
} // namespace HLVulkan

#endif //__HL_VULKAN_DEVICE_HPP__