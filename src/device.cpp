#include "device.hpp"

#include <set>
#include <string>

namespace HLVulkan {

  bool areDevExtSupported(VkPhysicalDevice device,
                          const std::vector<const char *> &devExt) {

    // Get list of all extensions supported by the device
    uint32_t extCount;
    if (vkEnumerateDeviceExtensionProperties(device, nullptr, &extCount,
                                             nullptr) != VK_SUCCESS) {
      return false;
    }
    std::vector<VkExtensionProperties> availableExtensions(extCount);
    if (vkEnumerateDeviceExtensionProperties(device, nullptr, &extCount,
                                             availableExtensions.data()) !=
        VK_SUCCESS) {
      return false;
    }

    std::set<std::string> requiredExtensions(devExt.begin(), devExt.end());

    for (const auto &extension : availableExtensions) {
      requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
  }

  Device::Device(const Surface &surface, const std::vector<Queue> &reqQueues)
      : queues(reqQueues) {

    queues = reqQueues;

    // ***** Pick a physical device *****

    auto instance = surface.getInstance();

    // Get list of compatible physical devices
    uint32_t phyDevCount = 0;
    VK_THROW(vkEnumeratePhysicalDevices(instance, &phyDevCount, nullptr),
             "failed to get list of physical devices");
    ASSERT_THROW(phyDevCount != 0, "failed to find GPUs with Vulkan support");
    std::vector<VkPhysicalDevice> phyDevs(phyDevCount);
    VK_THROW(vkEnumeratePhysicalDevices(instance, &phyDevCount, phyDevs.data()),
             "failed to get list of physical devices");

    // List of required extensions
    std::vector<const char *> reqExt{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    bool queueSupport;

    // Select a physical device
    for (const auto &phyDev : phyDevs) {

      // Reset structures
      scSupport = SCSupport();

      // Check for extension support
      if (!areDevExtSupported(phyDev, reqExt)) {
        continue;
      }
      // Check for swapchain support
      if (scSupport.querySupport(phyDev, *surface) != VK_SUCCESS) {
        continue;
      }
      // Check for queue support
      if (Queue::checkSupport(phyDev, *surface, queues, queueSupport) !=
          VK_SUCCESS) {
        continue;
      }

      physical = phyDev;
    }

    ASSERT_THROW(physical != VK_NULL_HANDLE, "failed to find a suitable GPU");

    // ***** Create logical device *****

    // Get set of queue families
    std::set<uint32_t> uniqueQueueFams = {};
    for (const auto &queue : queues) {
      uniqueQueueFams.insert(queue.family);
    }

    // Create one DeviceQueueInfo per queue family
    std::vector<VkDeviceQueueCreateInfo> queuesInfo;
    float queuePriority = 1.0f;
    for (auto queueFamily : uniqueQueueFams) {
      VkDeviceQueueCreateInfo qInfo = {};
      qInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      qInfo.queueFamilyIndex = queueFamily;
      qInfo.queueCount = 1;
      qInfo.pQueuePriorities = &queuePriority;
      queuesInfo.push_back(qInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};
    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queuesInfo.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queuesInfo.size());
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(reqExt.size());
    createInfo.ppEnabledExtensionNames = reqExt.data();
    createInfo.enabledLayerCount = 0;

    // Create the logical device
    VK_THROW(vkCreateDevice(physical, &createInfo, nullptr, &logical),
             "failed to create logical device!");

    // ***** Retrieve queue handles *****
    for (auto &queue : queues) {
      vkGetDeviceQueue(logical, queue.family, 0, &queue.handle);
    }
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
    return VK_FORMAT_UNDEFINED;
  }

  VkFormat Device::findDepthFormat() const {
    return findSupportedFormat({VK_FORMAT_D32_SFLOAT,
                                VK_FORMAT_D32_SFLOAT_S8_UINT,
                                VK_FORMAT_D24_UNORM_S8_UINT},
                               VK_IMAGE_TILING_OPTIMAL,
                               VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
  }

  VkDevice Device::operator*() const { return logical; }

  SCSupport Device::getSwapchainSupport() const { return scSupport; }

  std::optional<uint32_t> Device::getQueueFamily(VkFlags flags) const {
    for (const auto &queue : queues) {
      if (flags & queue.flags == queue.flags) {
        return queue.family;
      }
    }
    return {};
  }

  std::optional<uint32_t> Device::getPresentQueueFamily() const {
    for (const auto &queue : queues) {
      if (queue.present) {
        return queue.family;
      }
    }
    return {};
  }

  Device::Device(Device &&other)
      : physical(other.physical), logical(other.logical),
        scSupport(std::move(other.scSupport)), queues(std::move(other.queues)) {
    other.logical = VK_NULL_HANDLE;
  }

  Device &Device::operator=(Device &&other) {
    // Self-assignment detection
    if (&other != this) {
      physical = other.physical;
      logical = other.logical;
      scSupport = std::move(other.scSupport);
      queues = std::move(other.queues);

      other.logical = VK_NULL_HANDLE;
    }
    return *this;
  }

  Device::~Device() {
    if (logical != VK_NULL_HANDLE) {
      vkDestroyDevice(logical, nullptr);
    }
  }

} // namespace HLVulkan
