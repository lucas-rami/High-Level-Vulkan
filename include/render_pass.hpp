#ifndef __HL_VULKAN_RENDER_PASS_HPP__
#define __HL_VULKAN_RENDER_PASS_HPP__

#include "hl_vulkan.hpp"

namespace HLVulkan {

  class RenderPass {
  public:
    RenderPass(VkDevice device);

    RenderPass(const RenderPass &other) = delete;

    RenderPass &operator=(const RenderPass &other) = delete;

    RenderPass(RenderPass &&other);

    RenderPass &operator=(RenderPass &&other);

    VkResult create(const VkRenderPassCreateInfo &createInfo);

    virtual ~RenderPass();

  private:
    VkDevice device = VK_NULL_HANDLE;
    VkRenderPass handle = VK_NULL_HANDLE;
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_RENDER_PASS_HPP__