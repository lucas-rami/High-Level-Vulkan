#ifndef __HL_VULKAN_INSTANCE_HPP__
#define __HL_VULKAN_INSTANCE_HPP__

#include "hl_vulkan.hpp"

namespace HLVulkan {

  /**
   * @brief Packages a VkInstance.
   */
  class Instance {
  public:
    /**
     * @brief Constructs a Vulkan instance. Fails if GLFW extensions required to
     * interface with the window system aren't supported.
     *
     * @param appName The application name.
     */
    Instance(const std::string &appName);

    /**
     * @brief Destroys the created instance.
     */
    virtual ~Instance();

  private:
    // The instance
    VkInstance instance = VK_NULL_HANDLE; 
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_INSTANCE_HPP__