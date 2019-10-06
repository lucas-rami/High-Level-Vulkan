#ifndef __HL_VULKAN_WINDOW_HPP__
#define __HL_VULKAN_WINDOW_HPP__

#include "instance.hpp"

#include <GLFW/glfw3.h>
#include <string>

namespace HLVulkan {

  /**
   * @brief Packages a GLFWwindow. In any program there may only be one instance
   * of this class alive at any moment. This is because it also manages the GLFW
   * context by calling glfwInit() and glfwTerminate() when an instance is
   * respectively created and destroyed.
   */
  class Window {
  public:
    /**
     * @brief Creates a GLFW window of a particular size. Initializes GLFW.
     *
     * @param[in] windowName The window's name.
     * @param[in] width The window's width, in pixels.
     * @param[in] height The window's height, in pixels.
     */
    Window(const std::string &windowName, int width, int height);

    /**
     * @brief Sets/Overwrites the GLFW resize callback.
     *
     * @param[in] callback The new callback.
     */
    void setResizeCallback(GLFWframebuffersizefun callback) const;

    /**
     * @brief Sets/Overwrites the GLFW user pointer.
     *
     * @param[in] userPtr The new user pointer.
     */
    void setUserPointer(void *userPtr) const;

    /**
     * @brief Creates a Vulkan surface to render to.
     *
     * @param[in] instance The Vulkan instance that will be associated with the
     * surface.
     * @param[out] surface A Vulkan surface. On success, it is filled with a
     * valid handle to a VkSurface.
     *
     * @return VK_SUCCESS on success, a Vulkan error code otherwise.
     */
    VkResult createSurface(const Instance &instance,
                           VkSurfaceKHR &surface) const;

    /**
     * @brief Destroys the created window. Terminates the GLFW context.
     */
    virtual ~Window();

  private:
    // The window
    GLFWwindow *window = nullptr;
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_WINDOW_HPP__