#ifndef __HL_VULKAN_QUEUE_HPP__
#define __HL_VULKAN_QUEUE_HPP__

#include "hl_vulkan.hpp"

#include <optional>

namespace HLVulkan {

  /**
   * @brief Represents the result of evaluating a QueueRequest on a physical
   * device.
   */
  struct QueueResult {
    std::vector<std::pair<VkFlags, uint32_t>> families;
    std::optional<uint32_t> presentFamily;

    /**
     * @brief Creates an empty result. Typically, an empty result would be
     * passed to the QueueRequest.process() method.
     */
    QueueResult();

    /**
     * @brief Copy-constructor.
     */
    QueueResult(const QueueResult &other);

    /**
     * @brief Copy-assignment operator.
     */
    QueueResult &operator=(const QueueResult &other);

    /**
     * @brief Move-constructor.
     */
    QueueResult(const QueueResult &&other);

    /**
     * @brief Move-assignment operator.
     */
    QueueResult &operator=(const QueueResult &&other);
  };

  /**
   * @brief Requests a list of queues with certain capacities. This is used
   * when creating a Device.
   */
  struct QueueRequest {
    
    // The set of capabilities 
    std::vector<VkFlags> properties;
    // Whether we need a queue with presentation support
    bool presentSupport;

    /**
     * @brief Creates a request from a set of desired capabilities.
     *
     * @param[in] properties A list of VkFlags where each element define the set
     * of capacilities we wish a queue to have.
     * @param[in] presentSupport Whether we wish to have a queue with
     * presentation support.
     */
    QueueRequest(const std::vector<VkFlags> &properties, bool presentSupport);

    /**
     * @brief Copy-constructor.
     */
    QueueRequest(const QueueRequest &other);

    /**
     * @brief Copy-assignment operator.
     */
    QueueRequest &operator=(const QueueRequest &other);

    /**
     * @brief Move-constructor.
     */
    QueueRequest(const QueueRequest &&other);

    /**
     * @brief Move-assignment operator.
     */
    QueueRequest &operator=(const QueueRequest &&other);

    /**
     * @brief Processes the current request for a particular physical device.
     *
     * @param[in] device The physical device to query for queue capabilities.
     * @param[in] surface The surface to we wish to render to. This is useful
     * when querying for presentation support.
     * @param[out] res The result of this request for the device.
     *
     * @return VK_SUCCESS when all Vulkan functions executed successfully, an
     * error code otherwise.
     */
    VkResult process(VkPhysicalDevice device, VkSurfaceKHR surface,
                     QueueResult &res) const;

    /**
     * @brief Determines whether a QueueResult satisfies the request, i.e. it
     * has identified queue families with the desired capabilities.
     *
     * @param[in] The QueueResult to examine. Typically, it would have been
     * filled during a call to the process() method on the same request.
     *
     * @return true if the QueueResult satisfies the QueueRequest, false
     * otherwise.
     */
    bool satisfied(const QueueResult &res) const;
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_QUEUE_HPP__