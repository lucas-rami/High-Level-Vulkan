#ifndef __HL_VULKAN_DEVICE_HPP__
#define __HL_VULKAN_DEVICE_HPP__

#include "hl_vulkan.hpp"
#include "instance.hpp"
#include "queue.hpp"
#include "sc_support.hpp"
#include "surface.hpp"

namespace HLVulkan {

  /**
   * @brief Packages a VkDevice together with queue handles for the device.
   */
  class Device {
  public:
    /**
     * @brief Creates a logical device.
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
     * @brief Get the Vulkan device.
     *
     * @return The packaged Vulkan instance.
     */
    VkDevice operator*() const;

    /**
     * @brief Get swapchain support for this deive.
     *
     * @return The swapchain support associated to this device.
     */
    SCSupport getSwapchainSupport() const;

    /**
     * @brief Get the family of an allocated queue handle on the device which
     * has (at least) certain capabilities.
     *
     * @param[in] A set of capabiltiies for the queue family.
     *
     * @return The first queue family with certain capabilities for which there
     * is an allocated queue handle on the device. An empty value if there is no
     * such queue.
     */
    std::optional<uint32_t> getQueueFamily(VkFlags flags) const;

    /**
     * @brief Get the family of an allocated queue handle on the device which
     * has presentation support.
     *
     * @return The first queue family with presentation support for which there
     * is an allocated queue handle on the device. An empty value if there is no
     * such queue.
     */
    std::optional<uint32_t> getPresentQueueFamily() const;

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

    ~Device();

  private:
    // The physical device
    VkPhysicalDevice physical{VK_NULL_HANDLE};
    // The logical device
    VkDevice logical{VK_NULL_HANDLE};
    // Defines the swapchain support for the device
    SCSupport scSupport;
    // Queues instantiated from the device
    std::vector<Queue> queues;
  };
} // namespace HLVulkan

#endif //__HL_VULKAN_DEVICE_HPP__