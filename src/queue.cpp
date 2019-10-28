#include "queue.hpp"

#include <utility>

namespace HLVulkan {

  Queue::Queue(VkFlags flags, bool present) : flags(flags), present(present) {}

  Queue::Queue(VkFlags flags, bool present, uint32_t family, VkQueue handle)
      : flags(flags), present(present), family(family), handle(handle) {}

  VkResult Queue::checkSupport(VkPhysicalDevice device, VkSurfaceKHR surface,
                               std::vector<Queue> &queues, bool &supportOK) {

    // Get list of queue families for the device
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                             nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                             queueFamilies.data());

    // Check that the device has queue families with required flags
    for (auto &queue : queues) {
      // Queue family index
      uint32_t i = 0;
      bool found = false;

      for (const auto &family : queueFamilies) {
        if (family.queueCount > 0) {
          // Check for support
          if (family.queueFlags & queue.flags == queue.flags) {
            if (queue.present) {
              VkBool32 support = false;
              VK_RET(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface,
                                                          &support));
              if (support) {
                queue.family = i;
                found = true;
              }
            } else {
              queue.family = i;
              found = true;
            }
          }

          // Check whether the family had support
          if (found) {
            break;
          }
        }
        ++i;
      }

      // Check whether any family had support
      if (!found) {
        supportOK = false;
        return VK_SUCCESS;
      }
    }

    supportOK = true;
    return VK_SUCCESS;
  }

} // namespace HLVulkan