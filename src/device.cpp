#include "device.hpp"
#include "swapchain.hpp"

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

  Device::Device(Instance &&instance, Surface &&surface,
                 const QueueRequest &req)
      : instance(std::move(instance)), surface(std::move(surface)) {

    // ***** Pick a physical device *****

    // Get list of compatible physical devices
    uint32_t phyDevCount = 0;
    VK_THROW(vkEnumeratePhysicalDevices(*instance, &phyDevCount, nullptr),
             "failed to get list of physical devices");
    ASSERT_THROW(phyDevCount != 0, "failed to find GPUs with Vulkan support");
    std::vector<VkPhysicalDevice> phyDevs(phyDevCount);
    VK_THROW(
        vkEnumeratePhysicalDevices(*instance, &phyDevCount, phyDevs.data()),
        "failed to get list of physical devices");

    // List of required extensions
    std::vector<const char *> reqExt{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    // Select a physical device
    for (const auto &phyDev : phyDevs) {

      // Reset structures
      scSupport = SCSupport();
      queuesDesc = QueueResult();

      // Check for extension support
      if (!areDevExtSupported(phyDev, reqExt)) {
        continue;
      }
      // Check for swapchain support
      if (SwapChain::querySupport(phyDev, *surface, scSupport) != VK_SUCCESS) {
        continue;
      }
      // Check for queue support
      if (req.process(phyDev, *surface, queuesDesc) != VK_SUCCESS) {
        continue;
      }
      if (req.satisfied(queuesDesc)) {
        physical = phyDev;
        break;
      }
    }

    ASSERT_THROW(physical != VK_NULL_HANDLE, "failed to find a suitable GPU");

    // ***** Create logical device *****

    // Get set of queue families
    std::set<uint32_t> uniqueQueueFams = {};
    for (const auto &fam : queuesDesc.families) {
      uniqueQueueFams.insert(fam.second);
    }
    if (queuesDesc.presentFamily.has_value()) {
      uniqueQueueFams.insert(*queuesDesc.presentFamily);
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

    for (const auto &fam : queuesDesc.families) {
      VkQueue queue;
      vkGetDeviceQueue(logical, fam.second, 0, &queue);
      queues.push_back(queue);
    }
    if (queuesDesc.presentFamily.has_value()) {
      vkGetDeviceQueue(logical, *queuesDesc.presentFamily, 0, &presentQueue);
    }
  }

  Device::Device(Device &&other)
      : instance(std::move(other.instance)), surface(std::move(other.surface)),
        physical(other.physical), logical(other.logical),
        scSupport(std::move(other.scSupport)), queues(std::move(other.queues)),
        presentQueue(other.presentQueue) {
    other.logical = VK_NULL_HANDLE;
  }

  Device &Device::operator=(Device &&other) {
    // Self-assignment detection
    if (&other != this) {
      instance = std::move(instance);
      surface = std::move(surface);
      physical = other.physical;
      logical = other.logical;
      scSupport = std::move(other.scSupport);
      queues = std::move(other.queues);
      presentQueue = presentQueue;

      other.logical = VK_NULL_HANDLE;
    }
    return *this;
  }

  SCSupport Device::getSwapchainSupport() const { return scSupport; }

  std::optional<uint32_t> Device::getQueueFamily(VkFlags flags) const {
    for (const auto &family : queuesDesc.families) {
      if (family.first & flags == flags) {
        return family.second;
      }
    }
    return {};
  }

  std::optional<uint32_t> Device::getPresentQueueFamily() const {
    return queuesDesc.presentFamily;
  }

  uint32_t Device::findMemoryType(uint32_t typeFilter,
                                  VkMemoryPropertyFlags properties) const {

    // Query physical device for available types of memory
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physical, &memProperties);

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
    FAIL("failed to find supported format");
    return VK_FORMAT_UNDEFINED;
  }

  VkFormat Device::findDepthFormat() const {
    return findSupportedFormat({VK_FORMAT_D32_SFLOAT,
                                VK_FORMAT_D32_SFLOAT_S8_UINT,
                                VK_FORMAT_D24_UNORM_S8_UINT},
                               VK_IMAGE_TILING_OPTIMAL,
                               VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
  }

  Device::~Device() {
    if (logical != VK_NULL_HANDLE) {
      vkDestroyDevice(logical, nullptr);
    }
  }

} // namespace HLVulkan
