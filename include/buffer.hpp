#ifndef __HL_VULKAN_BUFFER_HPP__
#define __HL_VULKAN_BUFFER_HPP__

#include "command_pool.hpp"
#include "device.hpp"
#include "hl_vulkan.hpp"

namespace HLVulkan {

    class Buffer {

      private:
        Device device;
        VkDeviceSize size;
        VkBufferUsageFlags usage;
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkMemoryPropertyFlags memProperties = 0;

        VkResult bind();

      public:
        static VkResult createBuffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkBuffer &buffer);

        Buffer(Device device, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

        Buffer(Device device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

        VkResult allocateBuffer(VkDeviceSize size);

        VkResult mapAndCopy(const void *dataToCopy, size_t size);

        VkResult copyTo(const Buffer &dstBuffer, CommandPool &commandPool);

        VkBufferUsageFlags getUsageFlags();
        VkBuffer getBuffer();

        ~Buffer();
    };

} // namespace HLVulkan

#endif //__HL_VULKAN_BUFFER_HPP__
