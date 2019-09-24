#include "surface.hpp"
#include "hl_vulkan.hpp"

#include <GLFW/glfw3.h>

namespace HLVulkan {

  Surface::Surface(const Instance &instance, const Window &window)
      : instance(*instance) {
    VK_CHECK_FAIL(window.createSurface(instance, surface),
                  "failed to create surface");
  }

  VkSurfaceKHR Surface::operator*() const { return surface; }

  Surface::~Surface() { vkDestroySurfaceKHR(instance, surface, nullptr); }

} // namespace HLVulkan