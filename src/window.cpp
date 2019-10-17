#include "window.hpp"

namespace HLVulkan {

  Window::Window(const std::string &windowName, int width, int height) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window =
        glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
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

  void Window::setResizeCallback(GLFWframebuffersizefun callback) const {
    glfwSetFramebufferSizeCallback(window, callback);
  }

  void Window::setUserPointer(void *userPtr) const {
    glfwSetWindowUserPointer(window, userPtr);
  }

  GLFWwindow *Window::operator*() const { return window; }

  Window::~Window() {
    if (window != nullptr) {
      glfwDestroyWindow(window);
      glfwTerminate();
    }
  }

} // namespace HLVulkan