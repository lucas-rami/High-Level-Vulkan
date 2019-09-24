#ifndef __HL_VULKAN_WINDOW_HPP__
#define __HL_VULKAN_WINDOW_HPP__

#include <GLFW/glfw3.h>
#include <string>

namespace HLVulkan {

  /**
   * @brief Packages a GLFW window. In any program there may only be one
   * instance of this class alive at any moment. This is because it also manages
   * the GLFW context by calling glfwInit() and glfwTerminate() when an instance
   * is respectively created or destroyed. 
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
    void setResizeCallback(GLFWframebuffersizefun callback);

    /**
     * @brief Sets/Overwrites the GLFW user pointer.
     * 
     * @param[in] userPtr The new user pointer.
     */
    void setUserPointer(void* userPtr);

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