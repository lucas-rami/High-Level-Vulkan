#include "surface.hpp"
#include "hl_vulkan.hpp"

#include <GLFW/glfw3.h>

namespace HLVulkan {

  Surface::Surface(VkInstance instance, Window &&window)
      : instance(instance), window(std::move(window)) {
    VK_THROW(glfwCreateWindowSurface(instance, *window, nullptr, &surface),
             "failed to create surface");
  }

  Surface::Surface(Surface &&other)
      : instance(other.instance), window(std::move(other.window)),
        surface(other.surface) {
    other.surface = VK_NULL_HANDLE;
  }

  Surface &Surface::operator=(Surface &&other) {
    // Self-assignment detection
    if (&other != this) {
      instance = other.instance;
      window = std::move(other.window);
      surface = other.surface;

      other.surface = VK_NULL_HANDLE;
    }
    return *this;
  }

  VkSurfaceKHR Surface::operator*() const { return surface; }

  VkInstance Surface::getInstance() const { return instance; }

  GLFWwindow *Surface::getWindow() const { return *window; }

  Surface::~Surface() {
    if (surface != VK_NULL_HANDLE) {
      vkDestroySurfaceKHR(instance, surface, nullptr);
    }
  }

} // namespace HLVulkan