#ifndef __HL_VULKAN_COMMAND_POOL_HPP__
#define __HL_VULKAN_COMMAND_POOL_HPP__

#include "device.hpp"

namespace HLVulkan {

  /**
   * @brief Packages a VkCommandPool.
   */
  class CommandPool {

  public:
    /**
     * @brief Constructs a command pool on the device and allocate a fixed
     * number of command buffers from it. The pool submits commands to a queue
     * retrieved from the device
     *
     * @param[in] device The device to create the command pool from.
     * @param[in] desc A description of the capabilities of the queue to
     * retrieve from the device.
     * @param[in] count The number of command buffers to allocate from the pool.
     *
     * @throw std::runtime_error If a queue with desired capabilities cannot be
     * found on the device. If command pool or command buffer allocation fails
     * for some reason.
     */
    CommandPool(const Device &device, const Device::QueueDesc &desc,
                uint32_t count);

    /**
     * @brief Allocates a single-use command buffer from the pool and starts
     * recording on it.
     *
     * @param[out] command The command buffer.
     *
     * @return A Vulkan error code if a call to the Vulkan API fails, VK_SUCCESS
     * otherwise.
     */
    VkResult beginSingleTimeCommand(VkCommandBuffer &command) const;

    /**
     * @brief Stops recording on a single-use command buffer and submits it to
     * the underlying pool's queue. Waits for the queue to idle and frees the
     * buffer before returning.
     *
     * @param[in] command The command buffer.
     *
     * @return A Vulkan error code if a call to the Vulkan API fails, VK_SUCCESS
     * otherwise.
     */
    VkResult endSingleTimeCommand(VkCommandBuffer &command) const;

    /**
     * @brief Returns a command buffer
     *
     * @return A command buffer allocated from the pool.
     */
    VkCommandBuffer getCommandBuffer(size_t index) const;

    /**
     * @brief Returns the command pool.
     *
     * @return The command pool.
     */
    VkCommandPool getPool() const;

    /**
     * @brief Deleted copy-constructor.
     */
    CommandPool(CommandPool &other) = delete;

    /**
     * @brief Deleted copy-assignment operator.
     */
    CommandPool &operator=(CommandPool &other) = delete;

    /**
     * @brief Move-constructor.
     */
    CommandPool(CommandPool &&other);

    /**
     * @brief Move-assignment operator.
     */
    CommandPool &operator=(CommandPool &&other);

    virtual ~CommandPool();

  private:
    // The associated device
    VkDevice device{VK_NULL_HANDLE};
    // The queue to submit commands to
    VkQueue queue{VK_NULL_HANDLE};
    // The command pool
    VkCommandPool pool{VK_NULL_HANDLE};
    // Command buffers
    std::vector<VkCommandBuffer> commands;
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_COMMAND_POOL_HPP__