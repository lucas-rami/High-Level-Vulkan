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
     * @param[in] instance A Vulkan instance.
     * @param[in] window A GLFW window.
     */
    Surface(const Instance &instance, Window &&window);

    /**
     * @brief Get the size of the underlying window framebuffer.
     *
     * @param[out] width The framebuffer's width.
     * @param[out] height The framebuffer's height.
     */
    void getFramebufferSize(int &width, int &height) const;

    /**
     * @brief Get the Vulkan surface.
     *
     * @return The packaged Vulakn surface.
     */
    VkSurfaceKHR operator*() const;

    /**
     * @brief Returns the Vulkan instance associated to the surface.
     *
     * @return The associated Vulkan instance.
     */
    VkInstance getInstance() const;

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
     * @brief Destroys the created surface.
     */
    virtual ~Surface();

  private:
    // The instance associated with the surface
    VkInstance instance{VK_NULL_HANDLE};
    // The window associated with the surface
    Window window;
    // The Vulkan surface
    VkSurfaceKHR surface{VK_NULL_HANDLE};
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_SURFACE_HPP__