#ifndef __HL_VULKAN_RENDER_PASS_SPEC_HPP__
#define __HL_VULKAN_RENDER_PASS_SPEC_HPP__

#include "hl_vulkan.hpp"

namespace HLVulkan {

  /**
   * @brief A render pass specification. The virtual private methods of this
   * class define the content of the structures used during render pass
   * creation.
   */
  class RenderPassSpec {

  public:
    /**
     * @brief See RenderPassSpec::createAttachments.
     */
    void
    getAttachments(std::vector<VkAttachmentDescription> &attachments) const;

    /**
     * @brief See RenderPassSpec::createSubpasses.
     */
    void getSubpasses(std::vector<VkSubpassDescription> &subpasses) const;

    /**
     * @brief See RenderPassSpec::createDependencies.
     */
    void getDependencies(std::vector<VkSubpassDependency> &dependencies) const;

    /**
     * @brief Destructor.
     */
    virtual ~RenderPassSpec();

  private:
    /**
     * @brief Specifies the render pass attachments.
     *
     * @param[out] attachments The render pass attachments.
     */
    virtual void createAttachments(
        std::vector<VkAttachmentDescription> &attachments) const = 0;

    /**
     * @brief Specifies the render pass subpasses.
     *
     * @param[out] subpasses The render pass subpasses.
     */
    virtual void
    createSubpasses(std::vector<VkSubpassDescription> &subpasses) const = 0;

    /**
     * @brief Specifies the render pass dependencies.
     *
     * @param[out] dependencies The render pass dependencies.
     */
    virtual void createDependencies(
        std::vector<VkSubpassDependency> &dependencies) const = 0;
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_RENDER_PASS_SPEC_HPP__