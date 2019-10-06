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

#define VK_THROW(res, err) ASSERT_THROW((res) == VK_SUCCESS, err)

#define VK_FAIL(res, err) ASSERT_FAIL((res) == VK_SUCCESS, err)

#define VK_NULL(handle)                                                        \
  {                                                                            \
    if ((handle) != VK_NULL_HANDLE) {                                          \
      std::cout << "[FATAL]: " << #handle << " isn't VK_NULL_HANDLE"           \
                << " in " << __FILE__ << " at line " << __LINE__ << std::endl; \
      assert(false);                                                           \
    }                                                                          \
  }

#define VK_NOT_NULL(handle)\
  {                                                                            \
    if ((handle) == VK_NULL_HANDLE) {                                          \
      std::cout << "[FATAL]: " << #handle << " is VK_NULL_HANDLE"              \
                << " in " << __FILE__ << " at line " << __LINE__ << std::endl; \
      assert(false);                                                           \
    }                                                                          \
  }                                                    

#define VK_RET(b)                                                              \
  {                                                                            \
    VkResult res = (b);                                                        \
    if (res != VK_SUCCESS) {                                                   \
      return res;                                                              \
    }                                                                          \
  }

namespace HLVulkan {

  /**
   * @brief Checks whether a format has a stencil component.
   *
   * @param[in] format The format.
   *
   * @return true if the format has a stencil component.
   */
  bool hasStencilComponent(VkFormat format);

} // namespace HLVulkan

#endif //__HL_VULKAN_HPP__