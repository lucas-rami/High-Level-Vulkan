#ifndef __HL_VULKAN_FENCE_HPP__
#define __HL_VULKAN_FENCE_HPP__

#include "device.hpp"

namespace HLVulkan {

  /**
   * @brief Packages a VkFence.
   */
  class Fence {

  public:
    /**
     * @brief Creates a fence in the signaled state.
     * 
     * @param device The device to allocate the fence from.
     */
    Fence(const Device &device);

    /**
     * @brief Get the Vulkan fence.
     *
     * @return The packaged Vulkan fence.
     */
    VkFence operator*() const;

    /**
     * @brief Deleted copy-constructor.
     */
    Fence(Fence &other) = delete;

    /**
     * @brief Deleted copy-assignment operator.
     */
    Fence &operator=(Fence &other) = delete;

    /**
     * @brief Move-constructor.
     */
    Fence(Fence &&other);

    /**
     * @brief Move-assignment operator.
     */
    Fence &operator=(Fence &&other);

    virtual ~Fence();

  private:
    VkDevice device{VK_NULL_HANDLE};
    VkFence fence{VK_NULL_HANDLE};
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_FENCE_HPP__