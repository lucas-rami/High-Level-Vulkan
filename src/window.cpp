#include "window.hpp"

namespace HLVulkan {

  Window::Window(const std::string &windowName, int width, int height) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window =
        glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
  }

  void Window::setResizeCallback(GLFWframebuffersizefun callback) const {
    glfwSetFramebufferSizeCallback(window, callback);
  }

  void Window::setUserPointer(void *userPtr) const {
    glfwSetWindowUserPointer(window, userPtr);
  }

  VkResult Window::createSurface(const Instance &instance,
                                 VkSurfaceKHR &surface) const {
    return glfwCreateWindowSurface(*instance, window, nullptr, &surface);
  }

  Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
  }

} // namespace HLVulkan