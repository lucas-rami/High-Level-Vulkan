#ifndef __HL_VULKAN_IMAGE_HPP__
#define __HL_VULKAN_IMAGE_HPP__

#include "buffer.hpp"
#include "command_pool.hpp"
#include "device.hpp"
#include "hl_vulkan.hpp"

namespace HLVulkan {

  class Image {

  public:
    static VkResult create(VkDevice device, VkExtent2D extent, VkFormat format,
                           VkImageTiling tiling, VkImageUsageFlags usage,
                           VkImage &image);

    static VkResult createView(VkDevice device, VkImage image, VkFormat format,
                               VkImageAspectFlags aspect,
                               VkImageView &imageView);

    Image(VkPhysicalDevice phyDev, VkDevice device, VkExtent2D extent,
          VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
          VkImageAspectFlags aspect, VkMemoryPropertyFlags memProperties);

    Image(Image &other) = delete;

    Image& operator=(Image &other) = delete; 

    Image(Image &&other);

    Image& operator=(Image &&other); 

    VkResult copyTo(const Image &dstImage, CommandPool &commandPool);

    VkResult copyFromBuffer(VkImageLayout layout, Buffer &buffer,
                            CommandPool &commandPool);

    VkResult transitionImageLayout(VkImageLayout oldLayout,
                                   VkImageLayout newLayout,
                                   CommandPool &commandPool);

    VkImage getImage() const;
    VkImageView getView() const;
    VkDeviceMemory getMemory() const;

    virtual ~Image();

  private:
    VkDevice device;
    VkExtent2D extent;
    VkFormat format;
    VkImageTiling tiling;
    VkImageUsageFlags usage;
    VkImageAspectFlags aspect;
    VkMemoryPropertyFlags memProperties;

    VkImage image = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;
    VkImageView imageView = VK_NULL_HANDLE;

    VkResult bind(VkPhysicalDevice phyDev);
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_IMAGE_HPP__