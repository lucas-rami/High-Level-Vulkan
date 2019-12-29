#ifndef __HL_VULKAN_PIPELINE_SPEC_HPP__
#define __HL_VULKAN_PIPELINE_SPEC_HPP__

#include "hl_vulkan.hpp"

namespace HLVulkan {

  /**
   * @brief A pipeline specification. The virtual private methods of this class
   * define the content of the structures used to create a pipeline.
   */
  class PipelineSpec {

  public:
    /**
     * @brief See PipelineSpec::createInputAssembly.
     */
    void getInputAssembly(
        VkPipelineInputAssemblyStateCreateInfo &inputAssembly) const;

    /**
     * @brief See PipelineSpec::createViewports.
     */
    void getViewports(std::vector<VkViewport> &viewports) const;

    /**
     * @brief See PipelineSpec::createScissors.
     */
    void getScissors(std::vector<VkRect2D> &scissors) const;

    /**
     * @brief See PipelineSpec::createRasterizer.
     */
    void
    getRasterizer(VkPipelineRasterizationStateCreateInfo &rasterizer) const;

    /**
     * @brief See PipelineSpec::createMultisampling.
     */
    void
    getMultisampling(VkPipelineMultisampleStateCreateInfo &multisampling) const;

    /**
     * @brief See PipelineSpec::createColorBlending.
     */
    void getColorBlending(
        std::vector<VkPipelineColorBlendAttachmentState> &colorBlends) const;

    /**
     * @brief See PipelineSpec::createDescriptorSetLayouts.
     */
    void getDescriptorSetLayouts(
        std::vector<VkDescriptorSetLayout> &descSetLayouts) const;

    /**
     * @brief See PipelineSpec::createDepthStencil.
     */
    void
    getDepthStencil(VkPipelineDepthStencilStateCreateInfo &depthStencil) const;

    /**
     * @brief Destructor.
     */
    virtual ~PipelineSpec();

  private:
    /**
     * @brief Specifies the input assembly.
     *
     * @param[out] inputAssembly The input assembly.
     */
    virtual void createInputAssembly(
        VkPipelineInputAssemblyStateCreateInfo &inputAssembly) const = 0;

    /**
     * @brief Specifies the viewports.
     *
     * @param[out] viewports The viewports.
     */
    virtual void createViewports(std::vector<VkViewport> &viewports) const = 0;

    /**
     * @brief Specifies the scissors.
     *
     * @param[out] scissors The scissors.
     */
    virtual void createScissors(std::vector<VkRect2D> &scissors) const = 0;

    /**
     * @brief Specifies the rasterizer.
     *
     * @param[out] rasterizer The rasterizer.
     */
    virtual void createRasterizer(
        VkPipelineRasterizationStateCreateInfo &rasterizer) const = 0;

    /**
     * @brief Specifies the multisampling state.
     *
     * @param[out] multisampling The multisampling state.
     */
    virtual void createMultisampling(
        VkPipelineMultisampleStateCreateInfo &multisampling) const = 0;

    /**
     * @brief Specifies the color blend attachments.
     *
     * @param[out] colorBlends The color blend attachments.
     */
    virtual void createColorBlending(
        std::vector<VkPipelineColorBlendAttachmentState> &colorBlends)
        const = 0;

    /**
     * @brief Specifies the descriptor set layouts.
     *
     * @param[out] descSetLayouts The descriptor set layouts.
     */
    virtual void createDescriptorSetLayouts(
        std::vector<VkDescriptorSetLayout> &descSetLayouts) const = 0;

    /**
     * @brief Specifies the depth/stencil.
     *
     * @param[out] depthStencil The depth/stencil.
     */
    virtual void createDepthStencil(
        VkPipelineDepthStencilStateCreateInfo &depthStencil) const = 0;
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_PIPELINE_SPEC_HPP__