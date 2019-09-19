#ifndef __HL_VULKAN_COMMAND_POOL_HPP__
#define __HL_VULKAN_COMMAND_POOL_HPP__

#include "device.hpp"
#include "hl_vulkan.hpp"
#include "queue.hpp"

namespace HLVulkan {

    class CommandPool {

      public:
        CommandPool(Device device, Queue queue);

        CommandPool(Device device, Queue queue, uint32_t count);

        VkResult allocateCommandBuffers(uint32_t count);

        VkCommandBuffer beginSingleTimeCommands();

        VkResult endSingleTimeCommands(VkCommandBuffer commandBuffer);

        VkCommandBuffer getCommandBuffer(size_t index);

        VkCommandPool getPool();

        virtual ~CommandPool();

      private:
        const Device device;
        const Queue queue;

        VkCommandPool pool = VK_NULL_HANDLE;
        std::vector<VkCommandBuffer> commandBuffers;
    };

} // namespace HLVulkan

#endif //__HL_VULKAN_COMMAND_POOL_HPP__