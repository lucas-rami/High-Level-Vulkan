#ifndef __HL_VULKAN_COMMAND_POOL_HPP__
#define __HL_VULKAN_COMMAND_POOL_HPP__

#include "device.hpp"
#include "hl_vulkan.hpp"

namespace HLVulkan {

  class CommandPool {

  public:
    CommandPool(VkDevice device, uint32_t count, uint32_t queueFamily,
                VkQueue queue);

    CommandPool(const CommandPool &pool) = delete;

    CommandPool &operator=(const CommandPool &pool) = delete;

    VkResult allocateCommandBuffers(uint32_t count);

    VkResult beginSingleTimeCommand(VkCommandBuffer command);

    VkResult endSingleTimeCommand(VkCommandBuffer command);

    VkCommandBuffer getCommandBuffer(size_t index) const;

    VkCommandPool getPool() const;

    virtual ~CommandPool();

  private:
    VkDevice device;
    uint32_t queueFamily;
    VkQueue queue;

    VkCommandPool handle = VK_NULL_HANDLE;
    std::vector<VkCommandBuffer> commands;
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_COMMAND_POOL_HPP__