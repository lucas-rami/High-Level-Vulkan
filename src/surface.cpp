#include "surface.hpp"
#include "hl_vulkan.hpp"

#include <GLFW/glfw3.h>

namespace HLVulkan {

  Surface::Surface(const Instance &instance, const Window &window)
      : instance(*instance) {
    VK_CHECK_FAIL(window.createSurface(instance, surface),
                  "failed to create surface");
  }

  Surface::Surface(Surface &&other)
      : instance(other.instance), surface(other.surface) {
    other.instance = VK_NULL_HANDLE;
    other.surface = VK_NULL_HANDLE;
  }

  Surface &Surface::operator=(Surface &&other) {
    // Self-assignment detection
    if (&other != this) {
      instance = other.instance;
      surface = other.surface;

      other.instance = VK_NULL_HANDLE;
      other.surface = VK_NULL_HANDLE;
    }
    return *this;
  }

  VkSurfaceKHR Surface::operator*() const { return surface; }

  Surface::~Surface() {
    if (instance != VK_NULL_HANDLE && surface != VK_NULL_HANDLE) {
      vkDestroySurfaceKHR(instance, surface, nullptr);
    }
  }

} // namespace HLVulkan