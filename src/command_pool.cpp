#include "command_pool.hpp"

namespace HLVulkan {

  CommandPool::CommandPool(const Device &device, const Device::QueueDesc &desc,
                           uint32_t count)
      : device(*device) {

    // Get queue handle from device
    auto queueFamily = device.getQueueFamily(desc, &queue);
    ASSERT_THROW(queueFamily.has_value(), "failed to get queue from device")

    // Create the command pool
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = *queueFamily;
    VK_THROW(vkCreateCommandPool(this->device, &poolInfo, nullptr, &pool),
             "command pool creation failed");

    // Create command buffers
    commands.resize((size_t)count);
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = count;
    VK_THROW(
        !vkAllocateCommandBuffers(this->device, &allocInfo, commands.data()),
        "failed to allocate command buffers");
  }

  VkResult CommandPool::beginSingleTimeCommand(VkCommandBuffer &command) const {

    // Allocate the command buffer
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = pool;
    allocInfo.commandBufferCount = 1;
    VK_RET(vkAllocateCommandBuffers(device, &allocInfo, &command));

    // Begin recording
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    VK_RET(vkBeginCommandBuffer(command, &beginInfo));

    return VK_SUCCESS;
  }

  VkResult CommandPool::endSingleTimeCommand(VkCommandBuffer &command) const {

    // End recording
    VkResult ret;
    if ((ret = vkEndCommandBuffer(command)) != VK_SUCCESS) {
      // Failed to end recording, free buffer and return
      vkFreeCommandBuffers(device, pool, 1, &command);
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
      vkFreeCommandBuffers(device, pool, 1, &command);
      return ret;
    }

    // Wait for the queue to idle
    ret = vkQueueWaitIdle(queue);
    vkFreeCommandBuffers(device, pool, 1, &command);
    return ret;
  }

  VkCommandBuffer CommandPool::getCommandBuffer(size_t index) const {
    ASSERT_FAIL(index < commands.size(), "invalid index");
    return commands[index];
  }

  VkCommandPool CommandPool::getPool() const { return pool; }

  CommandPool::CommandPool(CommandPool &&other)
      : device(other.device), queue(other.queue), pool(other.pool),
        commands(std::move(other.commands)) {
    other.pool = VK_NULL_HANDLE;
  }

  CommandPool &CommandPool::operator=(CommandPool &&other) {
    // Self-assignment detection
    if (&other != this) {
      device = other.device;
      queue = other.queue;
      pool = other.pool;
      commands = std::move(other.commands);

      other.pool = VK_NULL_HANDLE;
    }
    return *this;
  }

  CommandPool::~CommandPool() {
    if (pool != VK_NULL_HANDLE) {
      vkDestroyCommandPool(device, pool, nullptr);
    }
  }

} // namespace HLVulkan
