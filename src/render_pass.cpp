#include "render_pass.hpp"

namespace HLVulkan {

  RenderPass::RenderPass(const Device &device, const RenderPassSpec &spec)
      : device(*device) {

    std::vector<VkAttachmentDescription> attachments;
    spec.getAttachments(attachments);
    std::vector<VkSubpassDescription> subpasses;
    spec.getSubpasses(subpasses);
    std::vector<VkSubpassDependency> dependencies;
    spec.getDependencies(dependencies);

    VkRenderPassCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.attachmentCount = static_cast<uint32_t>(attachments.size());
    info.pAttachments = attachments.data();
    info.subpassCount = static_cast<uint32_t>(subpasses.size());
    info.pSubpasses = subpasses.data();
    info.dependencyCount = static_cast<uint32_t>(dependencies.size());
    info.pDependencies = dependencies.data();

    VK_THROW(vkCreateRenderPass(this->device, &info, nullptr, &renderPass),
             "failed to create render pass");
  }

  RenderPass::RenderPass(RenderPass &&other)
      : device(other.device), renderPass(other.renderPass) {
    other.renderPass = VK_NULL_HANDLE;
  }

  RenderPass &RenderPass::operator=(RenderPass &&other) {
    // Self-assignment detection
    if (&other != this) {
      device = other.device;
      renderPass = other.renderPass;

      other.renderPass = VK_NULL_HANDLE;
    }
    return *this;
  };

  RenderPass::~RenderPass() {
    if (renderPass != VK_NULL_HANDLE) {
      vkDestroyRenderPass(device, renderPass, nullptr);
    }
  }

} // namespace HLVulkan
