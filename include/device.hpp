#ifndef __HL_VULKAN_DEVICE_HPP__
#define __HL_VULKAN_DEVICE_HPP__

#include <optional>

#include "hl_vulkan.hpp"
#include "instance.hpp"
#include "sc_support.hpp"
#include "surface.hpp"

namespace HLVulkan {

  /**
   * @brief Packages a VkDevice together with queue handles for the device.
   */
  class Device {
  public:
    /**
     * @brief Describes a queue's capabilities.
     */
    struct QueueDesc {
      // Flags for the queue
      VkFlags flags;
      // Whether the queue must support presentation operations
      bool presentSupport;

      /**
       * @brief A description specifying no capability.
       */
      QueueDesc() : flags(0), presentSupport(false) {}

      /**
       * @brief Consructs a queue description.
       *
       * @param[in] flags A set of flags describing the queue's capabilities.
       * @param[in] Whether the queue must support presentation operations.
       */
      QueueDesc(VkFlags flags, bool presentSupport)
          : flags(flags), presentSupport(presentSupport) {}
    };

    /**
     * @brief Creates a logical device.
     *
     * The created device is compliant with the passed list of QueueDesc , i.e.
     * its retrieved queues have the capabilities defined in the list..
     *
     * @param[in] surface The surface to render to (used to query for swapchain
     * and queue support).
     * @param[in] queuesDesc Describes queues we wish to retrieve from the
     * device.
     *
     * @throw std::runtime_error If for some reason a suitable device could not
     * be created.
     */
    Device(const Surface &surface, const std::vector<QueueDesc> &queuesDesc);

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
     * @param[in] desc Capabilities for the queue family.
     * @param[out] handle A user-provided pointer filled with a valid queue
     * handle if the pointer isn't null and a family was found.
     *
     * @return The first queue family with certain capabilities for which there
     * is an allocated queue handle on the device. An empty value if there is no
     * such queue.
     */
    std::optional<uint32_t> getQueueFamily(const QueueDesc &desc,
                                           VkQueue *handle = nullptr) const;

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

    virtual ~Device();

  private:
    /**
     * @brief Holds information about a queue.
     */
    struct Queue {
      // A description of the queue capabilities
      QueueDesc desc;
      // The queue family chosen for this queue
      uint32_t family{0};
      // A queue handle retrieved from the device being constructed
      VkQueue handle{VK_NULL_HANDLE};

      /**
       * @brief Constructs a Queue from a description of its capabilities. The
       * queue family and handle are left un-initialized.
       *
       * @param[in] desc A description of the queue's capabilities.
       */
      Queue(const QueueDesc &desc) : desc(desc) {}
    };

    // The physical device
    VkPhysicalDevice physical{VK_NULL_HANDLE};
    // The logical device
    VkDevice logical{VK_NULL_HANDLE};
    // Swapchain support for the device
    SCSupport scSupport;
    // Queues instantiated from the device
    std::vector<Queue> queues;

    /**
     * @brief Checks whether a physical device has support for a list of queues.
     *
     * @param[in] device The physical device.
     * @param[in] surface A Vulkan surface (used to check for presentation
     * support).
     * @param[out] supportOK Whether the physical device has support for the
     * requested queues.
     *
     * @return A Vulkan error code if a call to a function from the Vulkan API
     * failed, VK_SUCCESS otherwise.
     */
    VkResult checkSupport(VkPhysicalDevice device, VkSurfaceKHR surface,
                          bool &supportOK);
  };
} // namespace HLVulkan

#endif //__HL_VULKAN_DEVICE_HPP__