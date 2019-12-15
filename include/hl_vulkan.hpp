#ifndef __HL_VULKAN_HPP__
#define __HL_VULKAN_HPP__

#include <assert.h>
#include <iostream>
#include <vector>

#include <vulkan/vulkan.h>

#define ASSERT_FAIL(b, err)                                                    \
  {                                                                            \
    if (!(b)) {                                                                \
      std::cout << "[FATAL]: " << err << " in " << __FILE__ << " at line "     \
                << __LINE__ << std::endl;                                      \
      assert(false);                                                           \
    }                                                                          \
  }

#define ASSERT_THROW(b, err)                                                   \
  {                                                                            \
    if (!(b)) {                                                                \
      std::cout << "[EXCEPTION]: " << err << " in " << __FILE__ << " at line " \
                << __LINE__ << std::endl;                                      \
      throw std::runtime_error(err);                                           \
    }                                                                          \
  }

#define FAIL(err) ASSERT_FAIL(false, err)

#define VK_FAIL(res, err) ASSERT_FAIL((res) == VK_SUCCESS, err)

#define VK_THROW(res, err) ASSERT_THROW((res) == VK_SUCCESS, err)

#define VK_NULL(handle)                                                        \
  ASSERT_FAIL((handle) != VK_NULL_HANDLE, #handle << " isn't VK_NULL_HANDLE")

#define VK_NULL(handle)                                                        \
  ASSERT_FAIL((handle) == VK_NULL_HANDLE, #handle << " is VK_NULL_HANDLE")

#define VK_RET(b)                                                              \
  {                                                                            \
    VkResult res = (b);                                                        \
    if (res != VK_SUCCESS) {                                                   \
      return res;                                                              \
    }                                                                          \
  }

// Error code indicating that no suitable memory type could be found in a
// device.
#define NO_SUITABLE_MEMORY_TYPE 255

namespace HLVulkan {

  /**
   * @brief Checks whether a format has a stencil component.
   *
   * @param[in] format The format.
   *
   * @return true if the format has a stencil component.
   */
  bool hasStencilComponent(VkFormat format);

  /**
   * @brief Finds a suitable memory type on a device.
   *
   * @param[in] phyDevice A physical device.
   * @param[in] typeFilter A bitmask of supported memory types on the device.
   * Must be the memoryTypeBits field of a VkMemoryRequirements struct.
   * @param[in] properties A set of desired properties.
   *
   * @return A suitable memory type of minimum index if it exists,
   * NO_SUITABLE_MEMORY_TYPE otherwise.
   */
  uint32_t findMemoryType(VkPhysicalDevice phyDevice, uint32_t typeFilter,
                          VkMemoryPropertyFlags properties);

} // namespace HLVulkan

#endif //__HL_VULKAN_HPP__