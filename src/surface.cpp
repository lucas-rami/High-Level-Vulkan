#include "surface.hpp"
#include "hl_vulkan.hpp"

#include <GLFW/glfw3.h>

namespace HLVulkan {

  Surface::Surface(const Instance &instance, Window &&window)
      : instance(*instance), window(std::move(window)) {
    VK_THROW(window.createWindowSurface(*instance, surface),
             "failed to create surace");
  }

  void Surface::getFramebufferSize(int &width, int &height) const {
    window.getFramebufferSize(width, height);
  }

  VkInstance Surface::getInstance() const { return instance; }

  VkSurfaceKHR Surface::operator*() const { return surface; }

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

  Surface::~Surface() {
    if (surface != VK_NULL_HANDLE) {
      vkDestroySurfaceKHR(instance, surface, nullptr);
    }
  }

} // namespace HLVulkan