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
     * @param[in] instance The associated Vulkan instance.
     * @param[in] window The associated GLFW window.
     */
    Surface(VkInstance instance, Window &&window);

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

    /**
     * @brief Move-assignment operator.
     */
    Surface &operator=(Surface &&other);

    /**
     * @brief Retunrs the Vulkan instance associated to the surface.
     *
     * @return The associated Vulkan instance.
     */
    VkInstance getInstance() const;

    /**
     * @brief Retunrs a pointer to the GLFW window associated to the surface.
     *
     * @return A pointer to the associated GLFW window.
     */
    GLFWwindow *getWindow() const;

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
    VkInstance instance = VK_NULL_HANDLE;
    // The window associated with the surface
    Window window;
    // The Vulkan surface
    VkSurfaceKHR surface = VK_NULL_HANDLE;
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_SURFACE_HPP__