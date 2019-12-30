#include "image.hpp"

#include <optional>

namespace HLVulkan {

  Image::Image(const Device &device, VkExtent2D extent, VkFormat format,
               VkImageTiling tiling, VkImageUsageFlags usage,
               VkImageAspectFlags aspect, VkMemoryPropertyFlags properties)
      : device(*device), extent(extent), format(format) {

    // Create image
    VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = extent.width;
    imageInfo.extent.height = extent.height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    VK_THROW(vkCreateImage(this->device, &imageInfo, nullptr, &image),
             "failed to create image");

    // Get memory requirements
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(this->device, image, &memRequirements);

    // Allocate memory for image
    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(
        device.getPhysical(), memRequirements.memoryTypeBits, properties);
    VK_THROW(allocInfo.memoryTypeIndex == NO_SUITABLE_MEMORY_TYPE,
             "failed to find memory type");
    VK_THROW(vkAllocateMemory(this->device, &allocInfo, nullptr, &memory),
             "failed to allocate image memory");

    // Bind memory to image
    VK_THROW(vkBindImageMemory(this->device, image, memory, 0),
             "failed to bind image");

    // Create image view
    VK_THROW(createView(this->device, image, format, aspect, imageView),
             "failed to create iage view");
  }

  VkResult Image::copyTo(const Image &dstImage,
                         const CommandPool &commandPool) {

    VkCommandBuffer commandBuffer;
    VK_RET(commandPool.beginSingleTimeCommand(commandBuffer));

    VkImageCopy imageCopyRegion = {};
    imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageCopyRegion.srcSubresource.layerCount = 1;
    imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageCopyRegion.dstSubresource.layerCount = 1;
    imageCopyRegion.extent.width = extent.width;
    imageCopyRegion.extent.height = extent.height;
    imageCopyRegion.extent.depth = 1;

    vkCmdCopyImage(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                   dstImage.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1,
                   &imageCopyRegion);

    return commandPool.endSingleTimeCommand(commandBuffer);
  }

  VkResult Image::copyFromBuffer(VkImageLayout layout, const Buffer &srcBuffer,
                                 const CommandPool &commandPool) {

    VkCommandBuffer commandBuffer;
    VK_RET(commandPool.beginSingleTimeCommand(commandBuffer));

    // Create buffercopy information
    VkBufferImageCopy region = {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {extent.width, extent.height, 1};

    vkCmdCopyBufferToImage(commandBuffer, *srcBuffer, image, layout, 1,
                           &region);

    return commandPool.endSingleTimeCommand(commandBuffer);
  }

  VkResult Image::transitionImageLayout(VkImageLayout oldLayout,
                                        VkImageLayout newLayout,
                                        const CommandPool &commandPool) {

    // Create memory barrier
    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
      barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

      if (hasStencilComponent(format)) {
        barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
      }
    } else {
      barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    // Check that the transition is allowed
    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;
    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
        newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
      barrier.srcAccessMask = 0;
      barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

      sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
               newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

      sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
      destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
               newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
      barrier.srcAccessMask = 0;
      barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
                              VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

      sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
      destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    } else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL &&
               newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
      barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
      barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

      sourceStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
      destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else {
      ASSERT_FAIL(false, "unsupported layout transition")
      return VK_RESULT_MAX_ENUM;
    }

    // Create, record, and execute the command buffer
    VkCommandBuffer commandBuffer;
    VK_RET(commandPool.beginSingleTimeCommand(commandBuffer));
    vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0,
                         nullptr, 0, nullptr, 1, &barrier);
    VK_RET(commandPool.endSingleTimeCommand(commandBuffer));

    return VK_SUCCESS;
  }

  VkImage Image::operator*() const { return image; }
  VkImageView Image::getView() const { return imageView; }
  VkDeviceMemory Image::getMemory() const { return memory; }

  Image::Image(Image &&other)
      : device(other.device), extent(other.extent), format(other.format),
        image(other.image), memory(other.memory), imageView(other.imageView) {
    other.image = VK_NULL_HANDLE;
  }

  Image &Image::operator=(Image &&other) {
    // Self-assignment detection
    if (&other != this) {
      device = other.device;
      extent = other.extent;
      format = other.format;
      image = other.image;
      memory = other.memory;
      imageView = other.imageView;

      other.image = VK_NULL_HANDLE;
    }
    return *this;
  }

  Image::~Image() {
    if (image != VK_NULL_HANDLE) {
      vkDestroyImageView(device, imageView, nullptr);
      vkDestroyImage(device, image, nullptr);
      vkFreeMemory(device, memory, nullptr);
    }
  }

  VkResult Image::createView(VkDevice device, VkImage image, VkFormat format,
                             VkImageAspectFlags aspect,
                             VkImageView &imageView) {

    VkImageViewCreateInfo viewInfo = {};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspect;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    return vkCreateImageView(device, &viewInfo, nullptr, &imageView);
  }

} // namespace HLVulkan
