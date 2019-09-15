#include "hl_vulkan.hpp"

#include <string.h>

namespace HLVulkan {

    VkResult Buffer::createBuffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkBuffer &buffer) {

        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        return vkCreateBuffer(device, &bufferInfo, nullptr, &buffer);
    }

    Buffer::Buffer(Device device, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) : device(device), usage(usage), memProperties(properties) {}

    Buffer::Buffer(Device device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) : device(device), size(size), usage(usage), memProperties(properties) {
        VK_CHECK_FAIL(createBuffer(device.logical, size, usage, buffer), "buffer creation failed");
        VK_CHECK_FAIL(bind(), "buffer bind failed");
    }

    VkResult Buffer::allocateBuffer(VkDeviceSize size) {
        VK_CHECK_NULL(buffer);
        VK_CHECK_RET(createBuffer(device.logical, size, usage, buffer));
        return bind();
    }

    VkResult Buffer::bind() {

        VK_CHECK_NULL(memory);

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device.logical, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        std::optional memType = device.findMemoryType(memRequirements.memoryTypeBits, memProperties);
        if (memType) {
            allocInfo.memoryTypeIndex = *memType;
        } else {
            return VK_ERROR_FEATURE_NOT_PRESENT;
        }

        VK_CHECK_RET(vkAllocateMemory(device.logical, &allocInfo, nullptr, &memory));
        return vkBindBufferMemory(device.logical, buffer, memory, 0);
    }

    const VkResult Buffer::mapAndCopy(void *dataToCopy, size_t size) {

        VK_CHECK_NOT_NULL(memory);
        ASSERT_MSG(memProperties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, "memory is not mappable");

        void *data;
        VK_CHECK_RET(vkMapMemory(device.logical, memory, 0, size, 0, &data));
        memcpy(data, dataToCopy, size);
        vkUnmapMemory(device.logical, memory);

        return VK_SUCCESS;
    }

    const VkResult Buffer::copyTo(const Buffer &dstBuffer, CommandPool &commandPool) {

        //@ TODO: buffers must be bind to memory ?
        ASSERT_MSG(size <= dstBuffer.size, "destination buffer is bigger than source buffer");
        ASSERT_MSG((usage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT) != 0, "source buffer doesn't have required usage flag");
        ASSERT_MSG((dstBuffer.usage & VK_BUFFER_USAGE_TRANSFER_DST_BIT) != 0, "destination buffer doesn't have required usage flag");

        VkCommandBuffer commandBuffer = commandPool.beginSingleTimeCommands();
        VK_CHECK_NOT_NULL(commandBuffer);

        VkBufferCopy copyRegion = {};
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, buffer, dstBuffer.buffer, 1, &copyRegion);

        return commandPool.endSingleTimeCommands(commandBuffer);
    }

    const VkBufferUsageFlags Buffer::getUsageFlags() {
        return usage;
    }

    const VkBuffer Buffer::getBuffer() {
        return buffer;
    }

    Buffer::~Buffer() {
        vkDestroyBuffer(device.logical, buffer, nullptr);
        if (memory) {
            vkFreeMemory(device.logical, memory, nullptr);
        }
    }

} // namespace HLVulkan
