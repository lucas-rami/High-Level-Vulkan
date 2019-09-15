#include "vk_helper.hpp"

namespace HLVulkan {

    CommandPool::CommandPool(Device device, Queue queue) : device(device), queue(queue) {

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queue.family;

        VK_CHECK_FAIL(vkCreateCommandPool(device.logical, &poolInfo, nullptr, &pool), "command pool creation failed");
    }

    CommandPool::CommandPool(Device device, Queue queue, uint32_t count) : device(device), queue(queue) {

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queue.family;

        VK_CHECK_FAIL(vkCreateCommandPool(device.logical, &poolInfo, nullptr, &pool), "command pool creation failed");
        VK_CHECK_FAIL(allocateCommandBuffers(count), "failed to allocate command buffers");
    }

    VkResult CommandPool::allocateCommandBuffers(uint32_t count) {

        ASSERT_MSG(count != 0, "count must be strictly positive");
        ASSERT_MSG(commandBuffers.empty(), "allocate should only be called once per command pool"); // @TODO: this is a hardcore limitation

        // Resize the vector
        commandBuffers.resize((size_t)count);

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = pool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

        VkResult ret;
        // Resize the vector to 0 if the allocation fails
        if ((ret = vkAllocateCommandBuffers(device.logical, &allocInfo, commandBuffers.data())) != VK_SUCCESS) {
            commandBuffers.resize(0);
        }
        return ret;
    }

    VkCommandBuffer CommandPool::beginSingleTimeCommands() {

        VK_CHECK_NULL(singleTimeCommandBuffer);

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = pool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        VK_CHECK_RET_NULL(vkAllocateCommandBuffers(device.logical, &allocInfo, &commandBuffer));

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        VK_CHECK_RET_NULL(vkBeginCommandBuffer(commandBuffer, &beginInfo));

        return commandBuffer;
    }

    VkResult CommandPool::endSingleTimeCommands(VkCommandBuffer commandBuffer) {

        ASSERT_MSG(commandBuffer == singleTimeCommandBuffer, "argument buffer is invalid");
        singleTimeCommandBuffer = VK_NULL_HANDLE;

        // End recording
        VkResult ret;
        if ((ret = vkEndCommandBuffer(commandBuffer)) != VK_SUCCESS) {
            vkFreeCommandBuffers(device.logical, pool, 1, &commandBuffer);
            return ret;
        }

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        // Submit to the queue
        if ((ret = vkQueueSubmit(queue.queue, 1, &submitInfo, VK_NULL_HANDLE)) != VK_SUCCESS) {
            vkFreeCommandBuffers(device.logical, pool, 1, &commandBuffer);
            return ret;
        }

        // Wait for the queue to idle
        ret = vkQueueWaitIdle(queue.queue);
        vkFreeCommandBuffers(device.logical, pool, 1, &commandBuffer);
        return ret;
    }

    const VkCommandBuffer CommandPool::getCommandBuffer(size_t index) {
        ASSERT_MSG(index < commandBuffers.size(), "invalid index");
        return commandBuffers[index];
    }

    CommandPool::~CommandPool() { vkDestroyCommandPool(device.logical, pool, nullptr); }

} // namespace HLVulkan
