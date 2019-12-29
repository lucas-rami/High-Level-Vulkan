#include "pipeline_spec.hpp"

namespace HLVulkan {

  void PipelineSpec::getInputAssembly(
      VkPipelineInputAssemblyStateCreateInfo &inputAssembly) const {
    createInputAssembly(inputAssembly);
  }
  void PipelineSpec::getViewports(std::vector<VkViewport> &viewports) const {
    createViewports(viewports);
  }
  void PipelineSpec::getScissors(std::vector<VkRect2D> &scissors) const {
    createScissors(scissors);
  }
  void PipelineSpec::getRasterizer(
      VkPipelineRasterizationStateCreateInfo &rasterizer) const {
    createRasterizer(rasterizer);
  }
  void PipelineSpec::getMultisampling(
      VkPipelineMultisampleStateCreateInfo &multisampling) const {
    createMultisampling(multisampling);
  }
  void PipelineSpec::getColorBlending(
      std::vector<VkPipelineColorBlendAttachmentState> &colorBlends) const {
    createColorBlending(colorBlends);
  }
  void PipelineSpec::getDescriptorSetLayouts(
      std::vector<VkDescriptorSetLayout> &descSetLayouts) const {
    createDescriptorSetLayouts(descSetLayouts);
  }
  void PipelineSpec::getDepthStencil(
      VkPipelineDepthStencilStateCreateInfo &depthStencil) const {
    createDepthStencil(depthStencil);
  }
  
  PipelineSpec::~PipelineSpec() {}

} // namespace HLVulkan