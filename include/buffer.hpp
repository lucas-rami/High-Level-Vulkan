#ifndef __HL_VULKAN_BUFFER_HPP__
#define __HL_VULKAN_BUFFER_HPP__

#include "command_pool.hpp"
#include "device.hpp"

namespace HLVulkan {

  class Buffer {

  public:
    static VkResult create(VkDevice device, VkDeviceSize size,
                           VkBufferUsageFlags usage, VkBuffer &buffer);

    Buffer(VkPhysicalDevice phyDev, VkDevice device, VkDeviceSize size,
           VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

    Buffer(Buffer &other) = delete;

    Buffer &operator=(Buffer &other) = delete;

    Buffer(Buffer &&other);

    Buffer &operator=(Buffer &&other);

    VkResult mapAndCopy(const void *dataToCopy, size_t size);

    VkResult copyTo(const Buffer &dstBuffer, CommandPool &commandPool);

    VkBuffer operator*() const;

    VkBufferUsageFlags getUsageFlags() const;

    virtual ~Buffer();

  private:
    VkDevice device = VK_NULL_HANDLE;

    VkDeviceSize size;
    VkBufferUsageFlags usage;
    VkMemoryPropertyFlags memProperties;
    VkBuffer handle = VK_NULL_HANDLE;
    VkDeviceMemory memory = VK_NULL_HANDLE;

    VkResult bind(VkPhysicalDevice phyDev);
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_BUFFER_HPP__
