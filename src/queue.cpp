#include "queue.hpp"

#include <utility>

namespace HLVulkan {

  // ********** QueueResult **********

  QueueResult::QueueResult() : families(), presentFamily() {}

  QueueResult::QueueResult(const QueueResult &other)
      : families(other.families), presentFamily(presentFamily) {}

  QueueResult &QueueResult::operator=(const QueueResult &other) {
    // Self-assignment detection
    if (&other != this) {
      families = other.families;
      presentFamily = other.presentFamily;
    }
    return *this;
  }

  QueueResult::QueueResult(const QueueResult &&other)
      : families(std::move(other.families)),
        presentFamily(std::move(other.presentFamily)) {}

  QueueResult &QueueResult::operator=(const QueueResult &&other) {
    // Self-assignment detection
    if (&other != this) {
      families = std::move(other.families);
      presentFamily = std::move(other.presentFamily);
    }
    return *this;
  }

  // ********** QueueRequest **********

  QueueRequest::QueueRequest(const std::vector<VkFlags> &properties,
                             bool presentSupport)
      : properties(properties), presentSupport(presentSupport) {}

  QueueRequest::QueueRequest(const QueueRequest &other)
      : properties(other.properties), presentSupport(other.presentSupport) {}

  QueueRequest &QueueRequest::operator=(const QueueRequest &other) {
    // Self-assignment detection
    if (&other != this) {
      properties = other.properties;
      presentSupport = other.presentSupport;
    }
    return *this;
  }

  QueueRequest::QueueRequest(const QueueRequest &&other)
      : properties(std::move(other.properties)),
        presentSupport(other.presentSupport) {}

  QueueRequest &QueueRequest::operator=(const QueueRequest &&other) {
    // Self-assignment detection
    if (&other != this) {
      properties = std::move(other.properties);
      presentSupport = other.presentSupport;
    }
    return *this;
  }

  VkResult QueueRequest::process(VkPhysicalDevice device, VkSurfaceKHR surface,
                                 QueueResult &res) const {

    // Get list of queue families for the device
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                             nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                             queueFamilies.data());

    // Check that the device has queue families with required flags
    for (const auto &flags : properties) {
      uint32_t i = 0;
      for (const auto &family : queueFamilies) {
        if (family.queueCount > 0) {
          if (family.queueFlags & flags == flags) {
            res.families.push_back({flags, i});
            break;
          }
        }
        ++i;
      }
    }

    // Check whether we need a queue for presentation
    if (presentSupport) {
      uint32_t i = 0;
      for (const auto &family : queueFamilies) {
        if (family.queueCount > 0) {
          VkBool32 support = false;
          VK_CHECK_RET(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface,
                                                            &support));
          if (support) {
            res.presentFamily = i;
            break;
          }
        }
        ++i;
      }
    }

    return VK_SUCCESS;
  }

  bool QueueRequest::satisfied(const QueueResult &res) const {

    // Check that we found a queue family for each combination of properties
    if (properties.size() != res.families.size()) {
      return false;
    }
    for (size_t i = 0; i < properties.size(); ++i) {
      if (properties[i] != res.families[i].first) {
        return false;
      }
    }

    // Check that we found a queue for presentation if requested
    return presentSupport || res.presentFamily.has_value();
  }

} // namespace HLVulkan