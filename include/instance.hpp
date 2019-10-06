#ifndef __HL_VULKAN_INSTANCE_HPP__
#define __HL_VULKAN_INSTANCE_HPP__

#include <string>
#include <vulkan/vulkan.h>

namespace HLVulkan {

  /**
   * @brief Packages a VkInstance.
   */
  class Instance {
  public:
    /**
     * @brief Creates a Vulkan instance. Fails if GLFW extensions required to
     * interface with the window system aren't supported.
     *
     * @param[in] appName The application name.
     */
    Instance(const std::string &appName);

    /**
     * @brief Deleted copy-constructor.
     */
    Instance(Instance &other) = delete;

    /**
     * @brief Deleted copy-assignment operator.
     */
    Instance &operator=(Instance &other) = delete;

    /**
     * @brief Move-constructor.
     */
    Instance(Instance &&other);

    /**
     * @brief Move-assignment operator.
     */
    Instance &operator=(Instance &&other);

    /**
     * @brief Returns the packaged VkInstance.
     */
    VkInstance operator*() const;

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