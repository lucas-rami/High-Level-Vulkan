#include "render_pass.hpp"

namespace HLVulkan {

  RenderPass::RenderPass(VkDevice device) : device(device) {}

  RenderPass::RenderPass(RenderPass &&other)
      : device(other.device), handle(other.handle) {
    other.handle = VK_NULL_HANDLE;
  }

  RenderPass &RenderPass::operator=(RenderPass &&other) {
    // Self-assignment detection
    if (&other != this) {
      device = other.device;
      handle = other.handle;

      other.handle = VK_NULL_HANDLE;
    }
    return *this;
  };

  VkResult RenderPass::create(const VkRenderPassCreateInfo &createInfo) {
    ASSERT_FAIL(handle == VK_NULL_HANDLE, "render pass already created");
    return vkCreateRenderPass(device, &createInfo, nullptr, &handle);
  }

  RenderPass::~RenderPass() {
    if (handle != VK_NULL_HANDLE) {
      vkDestroyRenderPass(device, handle, nullptr);
    }
  }

} // namespace HLVulkan
