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
     * @brief Creates a Vukan surface using the GLFW window.
     *
     * @param[in] instance A Vulkan instance.
     * @param[out] surface A Vulakn surface.
     *
     * @return VK_SUCCESS if surface creation was successful, a Vulkan error
     * code otherwise.
     */
    VkResult createWindowSurface(VkInstance instance, VkSurfaceKHR &surface);

    /**
     * @brief Get the size of the window framebuffer.
     *
     * @param[out] width The framebuffer's width.
     * @param[out] height The framebuffer's height.
     */
    void getFramebufferSize(int &width, int &height) const;

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
     * @brief Deleted copy-constructor.
     */
    Window(Window &other) = delete;

    /**
     * @brief Deleted copy-assignment operator.
     */
    Window &operator=(Window &other) = delete;

    /**
     * @brief Move-constructor.
     */
    Window(Window &&other);

    /**
     * @brief Move-assignment operator.
     */
    Window &operator=(Window &&other);

    /**
     * @brief Destroys the created window. Terminates the GLFW context.
     */
    virtual ~Window();

  private:
    // The window
    GLFWwindow *window{nullptr};
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_WINDOW_HPP__