#include "buffer.hpp"

#include <optional>
#include <string.h>

namespace HLVulkan {

  Buffer::Buffer(const Device &device, VkDeviceSize size,
                 VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
      : device(*device), size(size), usage(usage), memProperties(properties) {

    // Create buffer
    VkBufferCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    info.size = size;
    info.usage = usage;
    info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    VK_THROW(vkCreateBuffer(this->device, &info, nullptr, &buffer),
             "failed to create buffer");

    // Get memory requirements
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(this->device, buffer, &memRequirements);

    // Allocate memory
    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(
        device.getPhysical(), memRequirements.memoryTypeBits, memProperties);
    VK_THROW(allocInfo.memoryTypeIndex == NO_SUITABLE_MEMORY_TYPE,
             "failed to find memory type");
    VK_THROW(vkAllocateMemory(this->device, &allocInfo, nullptr, &memory),
             "failed to allocate buffer memory");

    // Bind memory to buffer
    VK_THROW(vkBindBufferMemory(this->device, buffer, memory, 0),
             "failed to bind buffer");
  }

  VkResult Buffer::mapAndCopy(const void *dataToCopy, size_t size) {
    void *data;
    VK_RET(vkMapMemory(device, memory, 0, size, 0, &data));
    memcpy(data, dataToCopy, size);
    vkUnmapMemory(device, memory);
    return VK_SUCCESS;
  }

  VkResult Buffer::copyTo(const Buffer &dstBuffer,
                          const CommandPool &commandPool) {
    VkCommandBuffer commandBuffer;
    VK_RET(commandPool.beginSingleTimeCommand(commandBuffer));

    VkBufferCopy copyRegion = {};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, buffer, dstBuffer.buffer, 1, &copyRegion);

    return commandPool.endSingleTimeCommand(commandBuffer);
  }

  VkBuffer Buffer::operator*() const { return buffer; }

  VkBufferUsageFlags Buffer::getUsageFlags() const { return usage; }

  Buffer::Buffer(Buffer &&other)
      : device(other.device), size(other.size), usage(other.usage),
        memProperties(other.memProperties), buffer(other.buffer),
        memory(other.memory) {
    other.buffer = VK_NULL_HANDLE;
  }

  Buffer &Buffer::operator=(Buffer &&other) {
    // Self-assignment detection
    if (&other != this) {
      device = other.device;
      size = other.size;
      usage = other.usage;
      memProperties = other.memProperties;
      buffer = other.buffer;
      memory = other.memory;

      other.buffer = VK_NULL_HANDLE;
    }
    return *this;
  }

  Buffer::~Buffer() {
    if (buffer != VK_NULL_HANDLE) {
      vkDestroyBuffer(device, buffer, nullptr);
      vkFreeMemory(device, memory, nullptr);
    }
  }

} // namespace HLVulkan
