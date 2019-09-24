#include "window.hpp"

namespace HLVulkan {

  Window::Window(const std::string &windowName, int width, int height) {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window =
        glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
  }

  void Window::setResizeCallback(GLFWframebuffersizefun callback) {
    glfwSetFramebufferSizeCallback(window, callback);
  }

  void Window::setUserPointer(void* userPtr) {
    glfwSetWindowUserPointer(window, userPtr);
  }

  Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
  }

} // namespace HLVulkan