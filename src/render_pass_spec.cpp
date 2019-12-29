#include "render_pass_spec.hpp"

namespace HLVulkan {

  void RenderPassSpec::getAttachments(
      std::vector<VkAttachmentDescription> &attachments) const {
    return createAttachments(attachments);
  }
  void RenderPassSpec::getSubpasses(
      std::vector<VkSubpassDescription> &subpasses) const {
    return createSubpasses(subpasses);
  }
  void RenderPassSpec::getDependencies(
      std::vector<VkSubpassDependency> &dependencies) const {
    return createDependencies(dependencies);
  }

  RenderPassSpec::~RenderPassSpec() {}

} // namespace HLVulkan