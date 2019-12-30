#ifndef __HL_VULKAN_BUFFER_HPP__
#define __HL_VULKAN_BUFFER_HPP__

#include "command_pool.hpp"
#include "device.hpp"

namespace HLVulkan {

  /**
   * @brief Packages a VkBuffer together with a VkDeviceMemory.
   */
  class Buffer {

  public:
    /**
     * @brief Creates a buffer, allocates memory for it, and binds the buffer
     * to its memory.
     *
     * @param[in] device The device to allocate the buffer from.
     * @param[in] size The buffer size.
     * @param[in] usage Usage flags for the buffer.
     * @param[in] properties Property flags for the buffer memory.
     *
     * @throw std::runtime_error If buffer creation fails for some reason.
     */
    Buffer(const Device &device, VkDeviceSize size, VkBufferUsageFlags usage,
           VkMemoryPropertyFlags properties);

    /**
     * @brief Maps buffer to host-visible memory and copies data to it.
     * 
     * @param[in] dataToCopy A pointer to the memory region containing data to
     * copy to the buffer.
     * @param[in] size The number of bytes to copy to the buffer, starting at
     * the address contained in dataToCopy.
     * 
     * @return A Vulkan error code if a call to the Vulkan API fails, VK_SUCCESS
     * otherwise.
     */
    VkResult mapAndCopy(const void *dataToCopy, size_t size);

    /**
     * @brief Copies the buffer's content to another buffer.
     * 
     * @param[in] dstBuffer The destination buffer.
     * @param[in] commandPool The command pool to use to submit the memory copy
     * command.
     * 
     * @return A Vulkan error code if a call to the Vulkan API fails, VK_SUCCESS
     * otherwise.
     */
    VkResult copyTo(const Buffer &dstBuffer, const CommandPool &commandPool);

    /**
     * @brief Get the Vulkan buffer.
     *
     * @return The packaged Vulkan buffer.
     */
    VkBuffer operator*() const;

    /**
     * @brief Get the buffer usage flags.
     *
     * @return The buffer usage flags.
     */
    VkBufferUsageFlags getUsageFlags() const;

    /**
     * @brief Deleted copy-constructor.
     */
    Buffer(Buffer &other) = delete;

    /**
     * @brief Deleted copy-assignment operator.
     */
    Buffer &operator=(Buffer &other) = delete;

    /**
     * @brief Move-constructor.
     */
    Buffer(Buffer &&other);

    /**
     * @brief Move-assignment operator.
     */
    Buffer &operator=(Buffer &&other);

    /**
     * @brief Destructor.
     */
    virtual ~Buffer();

  private:
    // The device to allocate the buffer from
    VkDevice device;
    // The buffer size
    VkDeviceSize size;
    // Flags indicating the buffer intended usage
    VkBufferUsageFlags usage;
    // Flags indicating properties for the buffer memory
    VkMemoryPropertyFlags memProperties;

    // The buffer handle
    VkBuffer buffer{VK_NULL_HANDLE};
    // The buffer memory handle
    VkDeviceMemory memory{VK_NULL_HANDLE};
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_BUFFER_HPP__
