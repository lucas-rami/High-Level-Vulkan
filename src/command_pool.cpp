#include "command_pool.hpp"

namespace HLVulkan {

  CommandPool::CommandPool(VkDevice device, uint32_t count,
                           uint32_t queueFamily, VkQueue queue)
      : device(device), queueFamily(queueFamily), queue(queue) {

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamily;

    VK_THROW(vkCreateCommandPool(device, &poolInfo, nullptr, &handle),
             "command pool creation failed");
    VK_THROW(allocateCommandBuffers(count),
             "failed to allocate command buffers");
  }

  VkResult CommandPool::allocateCommandBuffers(uint32_t count) {

    ASSERT_FAIL(count != 0, "count must be strictly positive");
    // @TODO this is a big limitation
    ASSERT_FAIL(commands.empty(),
                "allocate should only be called once per command pool");

    // Resize the vector
    commands.resize((size_t)count);

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = handle;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = count;

    VkResult ret;
    if ((ret = vkAllocateCommandBuffers(device, &allocInfo, commands.data())) !=
        VK_SUCCESS) {
      // Resize the vector to 0 if the allocation fails
      commands.resize(0);
    }
    return ret;
  }

  VkResult CommandPool::beginSingleTimeCommand(VkCommandBuffer command) {

    // Allocate the command buffer
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = handle;
    allocInfo.commandBufferCount = 1;
    VK_RET(vkAllocateCommandBuffers(device, &allocInfo, &command));

    // Begin recording
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    VK_RET(vkBeginCommandBuffer(command, &beginInfo));

    return VK_SUCCESS;
  }

  VkResult CommandPool::endSingleTimeCommand(VkCommandBuffer command) {

    // End recording
    VkResult ret;
    if ((ret = vkEndCommandBuffer(command)) != VK_SUCCESS) {
      // Failed to end recording, free buffer and return
      vkFreeCommandBuffers(device, handle, 1, &command);
      return ret;
    }

    // Submit to the queue
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &command;
    if ((ret = vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE)) !=
        VK_SUCCESS) {
      // Queue submission failed, free buffer and return
      vkFreeCommandBuffers(device, handle, 1, &command);
      return ret;
    }

    // Wait for the queue to idle
    ret = vkQueueWaitIdle(queue);
    vkFreeCommandBuffers(device, handle, 1, &command);
    return ret;
  }

  VkCommandBuffer CommandPool::getCommandBuffer(size_t index) const {
    ASSERT_FAIL(index < commands.size(), "invalid index");
    return commands[index];
  }

  VkCommandPool CommandPool::getPool() const { return handle; }

  CommandPool::~CommandPool() {
    if (handle != VK_NULL_HANDLE) {
      vkDestroyCommandPool(device, handle, nullptr);
    }
  }

} // namespace HLVulkan
