#ifndef __HL_VULKAN_RENDER_PASS_FACTORY_HPP__
#define __HL_VULKAN_RENDER_PASS_FACTORY_HPP__

#include "render_pass.hpp"

namespace HLVulkan {

  class RenderPassFactory {

  public:
    RenderPassFactory() = delete;

    template <class RenderPassSpec>
    static VkResult createRenderPass(const RenderPassSpec &spec,
                                     RenderPass &renderPass) {

      auto attachments = spec.getAttachments();
      auto subpasses = spec.getSubpasses();
      auto dependencies = spec.getDependencies();

      VkRenderPassCreateInfo renderPassInfo = {};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
      renderPassInfo.attachmentCount =
          static_cast<uint32_t>(attachments.size());
      renderPassInfo.pAttachments = attachments.data();
      renderPassInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
      renderPassInfo.pSubpasses = subpasses.data();
      renderPassInfo.dependencyCount =
          static_cast<uint32_t>(dependencies.size());
      renderPassInfo.pDependencies = dependencies.data();

      return renderPass.create(renderPassInfo);
    }
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_RENDER_PASS_FACTORY_HPP__