#include "buffer.hpp"

#include <optional>
#include <string.h>

namespace HLVulkan {

  VkResult Buffer::create(VkDevice device, VkDeviceSize size,
                          VkBufferUsageFlags usage, VkBuffer &buffer) {

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    return vkCreateBuffer(device, &bufferInfo, nullptr, &buffer);
  }

  Buffer::Buffer(VkPhysicalDevice phyDev, VkDevice device, VkDeviceSize size,
                 VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
      : device(device), size(size), usage(usage), memProperties(properties) {
    VK_THROW(Buffer::create(device, size, usage, handle),
             "buffer creation failed");
    VK_THROW(bind(phyDev), "buffer bind failed");
  }

  Buffer::Buffer(Buffer &&other)
      : device(other.device), size(other.size), usage(other.usage),
        memProperties(other.memProperties), handle(other.handle),
        memory(other.memory) {
    other.handle = VK_NULL_HANDLE;
  }

  Buffer &Buffer::operator=(Buffer &&other) {
    // Self-assignment detection
    if (&other != this) {
      device = other.device;
      size = other.size;
      usage = other.usage;
      memProperties = other.memProperties;
      handle = other.handle;
      memory = other.memory;

      other.handle = VK_NULL_HANDLE;
    }
    return *this;
  }

  VkResult Buffer::bind(VkPhysicalDevice phyDev) {

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, handle, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex =
        findMemoryType(phyDev, memRequirements.memoryTypeBits, memProperties);
    if (allocInfo.memoryTypeIndex == NO_SUITABLE_MEMORY_TYPE) {
      return VK_ERROR_FEATURE_NOT_PRESENT;
    }

    VK_RET(vkAllocateMemory(device, &allocInfo, nullptr, &memory));
    return vkBindBufferMemory(device, handle, memory, 0);
  }

  VkResult Buffer::mapAndCopy(const void *dataToCopy, size_t size) {

    ASSERT_FAIL(memProperties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
                "memory is not mappable");

    void *data;
    VK_RET(vkMapMemory(device, memory, 0, size, 0, &data));
    memcpy(data, dataToCopy, size);
    vkUnmapMemory(device, memory);

    return VK_SUCCESS;
  }

  VkResult Buffer::copyTo(const Buffer &dstBuffer, CommandPool &commandPool) {

    // @TODO: buffers must be bind to memory ?
    ASSERT_FAIL(size <= dstBuffer.size,
                "destination buffer is bigger than source buffer");
    ASSERT_FAIL((usage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT) != 0,
                "source buffer doesn't have required usage flag");
    ASSERT_FAIL((dstBuffer.usage & VK_BUFFER_USAGE_TRANSFER_DST_BIT) != 0,
                "destination buffer doesn't have required usage flag");

    VkCommandBuffer commandBuffer;
    VK_RET(commandPool.beginSingleTimeCommand(commandBuffer));

    VkBufferCopy copyRegion = {};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, handle, dstBuffer.handle, 1, &copyRegion);

    return commandPool.endSingleTimeCommand(commandBuffer);
  }

  VkBuffer Buffer::operator*() const { return handle; }

  VkBufferUsageFlags Buffer::getUsageFlags() const { return usage; }

  Buffer::~Buffer() {
    if (handle != VK_NULL_HANDLE) {
      vkDestroyBuffer(device, handle, nullptr);
      vkFreeMemory(device, memory, nullptr);
    }
  }

} // namespace HLVulkan
