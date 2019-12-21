#include "swapchain.hpp"
#include "hl_vulkan.hpp"
#include "image.hpp"

namespace HLVulkan {

  VkSurfaceFormatKHR
  chooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &candidates) {

    // Surface has no preferred format (we are free to choose)
    if (candidates.size() == 1 && candidates[0].format == VK_FORMAT_UNDEFINED) {
      return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    // Select preferred format if it is available
    for (const auto &format : candidates) {
      if (format.format == VK_FORMAT_B8G8R8A8_UNORM &&
          format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        return format;
      }
    }

    // Otherwise take first proposed format
    return candidates[0];
  }

  VkPresentModeKHR
  choosePresentMode(const std::vector<VkPresentModeKHR> &candidates) {
    // Vertical sync (double buffering)
    auto bestMode = VK_PRESENT_MODE_FIFO_KHR;

    for (const auto &availablePresentMode : candidates) {
      if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
        // Triple buffering
        return availablePresentMode;
      } else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
        // No buffering
        bestMode = availablePresentMode;
      }
    }

    // VK_PRESENT_MODE_MAILBOX_KHR > VK_PRESENT_MODE_IMMEDIATE_KHR >
    // VK_PRESENT_MODE_FIFO_KHR
    return bestMode;
  }

  VkExtent2D chooseExtent(const Surface &surface,
                          const VkSurfaceCapabilitiesKHR &capabilities) {
    if (capabilities.currentExtent.width !=
        std::numeric_limits<uint32_t>::max()) {
      return capabilities.currentExtent;
    } else {
      int width, height;
      surface.getFramebufferSize(width, height);

      VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                                 static_cast<uint32_t>(height)};

      actualExtent.width = std::max(
          capabilities.minImageExtent.width,
          std::min(capabilities.maxImageExtent.width, actualExtent.width));
      actualExtent.height = std::max(
          capabilities.minImageExtent.height,
          std::min(capabilities.maxImageExtent.height, actualExtent.height));

      return actualExtent;
    }
  }

  SwapChain::SwapChain(const Surface &surface, const Device &device) {

    // Choose swapchain characteristics
    SCSupport deviceSupport = device.getSwapchainSupport();
    surfaceFormat = chooseSurfaceFormat(deviceSupport.formats);
    presentMode = choosePresentMode(deviceSupport.presentModes);
    extent = chooseExtent(surface, deviceSupport.capabilities);

    // Decide on the number of images in the swapchain
    uint32_t imageCount = deviceSupport.capabilities.minImageCount + 1;
    if (deviceSupport.capabilities.maxImageCount > 0 &&
        imageCount > deviceSupport.capabilities.maxImageCount) {
      imageCount = deviceSupport.capabilities.maxImageCount;
    }

    // Get queue families
    uint32_t queueFamilyIndices[2];
    auto graphics =
        device.getQueueFamily(Device::QueueDesc(VK_QUEUE_GRAPHICS_BIT, false));
    auto present = device.getQueueFamily(Device::QueueDesc(0, true));
    ASSERT_THROW(graphics.has_value() && present.has_value(),
                 "device doesn't have a graphics or present queue");
    queueFamilyIndices[0] = *graphics;
    queueFamilyIndices[1] = *present;

    // Create structures for vkCreateSwapchainKHR()
    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = *surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    if (*graphics != *present) {
      createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT; // Best perf
      createInfo.queueFamilyIndexCount = 2;
      createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
      createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // Worst perf
    }
    createInfo.preTransform = deviceSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    // Create the swapchain and fill in &swapChain
    VK_THROW(vkCreateSwapchainKHR(*device, &createInfo, nullptr, &swapchain),
             "failed to create swapchain");

    // Retrieve the swapchain's images
    uint32_t count;
    vkGetSwapchainImagesKHR(*device, swapchain, &count, nullptr);
    images.resize(count);
    vkGetSwapchainImagesKHR(*device, swapchain, &count, images.data());

    // Retrieve the swapchain's image views
    views.resize(count);
    for (size_t i = 0; i < count; ++i) {
      VK_THROW(Image::createView(*device, images[i], surfaceFormat.format,
                                 VK_IMAGE_ASPECT_COLOR_BIT, views[i]),
               "failed to create image view");
    }
  }

} // namespace HLVulkan
