#ifndef __HL_VULKAN_PIPELINE_FACTORY_HPP__
#define __HL_VULKAN_PIPELINE_FACTORY_HPP__

#include "pipeline.hpp"
#include "pipeline_spec.hpp"
#include "shader.hpp"
#include "vertex_spec.hpp"

namespace HLVulkan {

  class PipelineFactory {

  public:
    PipelineFactory() = delete;

    static VkResult createGraphicPipeline(const VertexSpec &vertSpec,
                                          const PipelineSpec &pipeSpec,
                                          const std::vector<Shader> &shaders,
                                          VkRenderPass renderPass,
                                          Pipeline &pipeline);

  };

} // namespace HLVulkan

#endif //__HL_VULKAN_PIPELINE_FACTORY_HPP__