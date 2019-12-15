#include "window.hpp"

namespace HLVulkan {

  Window::Window(const std::string &windowName, int width, int height) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window =
        glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
  }

  VkResult Window::createWindowSurface(VkInstance instance,
                                       VkSurfaceKHR &surface) {
    return glfwCreateWindowSurface(instance, window, nullptr, &surface);
  }

  void Window::getFramebufferSize(int &width, int &height) const {
    glfwGetFramebufferSize(window, &width, &height);
  }

  void Window::setResizeCallback(GLFWframebuffersizefun callback) const {
    glfwSetFramebufferSizeCallback(window, callback);
  }

  void Window::setUserPointer(void *userPtr) const {
    glfwSetWindowUserPointer(window, userPtr);
  }

  Window::Window(Window &&other) : window(other.window) {
    other.window = nullptr;
  }

  Window &Window::operator=(Window &&other) {
    // Self-assignment detection
    if (&other != this) {
      window = other.window;

      other.window = nullptr;
    }
    return *this;
  }

  Window::~Window() {
    if (window != nullptr) {
      glfwDestroyWindow(window);
      glfwTerminate();
    }
  }

} // namespace HLVulkan