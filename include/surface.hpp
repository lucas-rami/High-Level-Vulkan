#ifndef __HL_VULKAN_SURFACE_HPP__
#define __HL_VULKAN_SURFACE_HPP__

#include "instance.hpp"
#include "window.hpp"

#include <vulkan/vulkan.h>

namespace HLVulkan {

  /**
   * @brief Packages a VkSurfaceKHR.
   */
  class Surface {
  public:
    /**
     * @brief Creates a Vulkan surface.
     *
     * @param[in] instance The associated instance.
     * @param[in] window The associated GLFW window.
     */
    Surface(Instance &&instance, Window &&window);

    /**
     * @brief Deleted copy-constructor.
     */
    Surface(Surface &other) = delete;

    /**
     * @brief Deleted copy-assignment operator.
     */
    Surface &operator=(Surface &other) = delete;

    /**
     * @brief Move-constructor.
     */
    Surface(Surface &&other);

    Instance& getInstance();

    Window& getWindow();

    /**
     * @brief Move-assignment operator.
     */
    Surface &operator=(Surface &&other);

    /**
     * @brief Returns the packaged VkSurfaceKHR.
     */
    VkSurfaceKHR operator*() const;

    /**
     * @brief Destroys the created surface.
     */
    virtual ~Surface();

  private:
    // The instance associated with the surface
    Instance instance;
    // The window associated with the surface
    Window window;
    // The Vulkan surface
    VkSurfaceKHR surface = VK_NULL_HANDLE;
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_SURFACE_HPP__