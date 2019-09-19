#include "pipeline_spec.hpp"

namespace HLVulkan {

    VkPipelineInputAssemblyStateCreateInfo PipelineSpec::getInputAssembly() const { return createInputAssembly(); }
    std::vector<VkViewport> PipelineSpec::getViewports() const { return createViewports(); }
    std::vector<VkRect2D> PipelineSpec::getScissors() const { return createScissors(); }
    VkPipelineRasterizationStateCreateInfo PipelineSpec::getRasterizer() const { return createRasterizer(); }
    VkPipelineMultisampleStateCreateInfo PipelineSpec::getMultisampling() const { return createMultisampling(); }
    std::vector<VkPipelineColorBlendAttachmentState> PipelineSpec::getColorBlending() const { return createColorBlending(); }
    std::vector<VkDescriptorSetLayout> PipelineSpec::getDescriptorSetLayouts() const { return createDescriptorSetLayouts(); }
    VkPipelineDepthStencilStateCreateInfo PipelineSpec::getDepthStencil() const { return createDepthStencil(); }

    PipelineSpec::~PipelineSpec() {}

} // namespace HLVulkan