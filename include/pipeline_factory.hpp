#ifndef __HL_VULKAN_PIPELINE_FACTORY_HPP__
#define __HL_VULKAN_PIPELINE_FACTORY_HPP__

#include "pipeline.hpp"
#include "pipeline_spec.hpp"
#include "shader.hpp"

namespace HLVulkan {

  class PipelineFactory {

  public:
    PipelineFactory() = delete;

    template <class VertexFormat, class PipelineSpec>
    static VkResult createGraphicPipeline(const VertexFormat &vertFormat,
                                          const PipelineSpec &spec,
                                          const std::vector<Shader> &shaders,
                                          VkRenderPass renderPass,
                                          Pipeline &pipeline) {

      auto bindingDescription = vertFormat.getBindingDescription();
      auto attributeDescriptions = vertFormat.getAttributeDescriptions();

      // Vertex input
      VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
      vertexInputInfo.sType =
          VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
      vertexInputInfo.vertexBindingDescriptionCount = 1;
      vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
      vertexInputInfo.vertexAttributeDescriptionCount =
          static_cast<uint32_t>(attributeDescriptions.size());
      vertexInputInfo.pVertexAttributeDescriptions =
          attributeDescriptions.data();

      // Input assembly
      VkPipelineInputAssemblyStateCreateInfo inputAssembly =
          spec.getInputAssembly();

      // Viewports
      std::vector<VkViewport> viewports = spec.getViewports();

      // Scissors
      std::vector<VkRect2D> scissors = spec.getScissors();

      // Viewport state
      VkPipelineViewportStateCreateInfo viewportState = {};
      viewportState.sType =
          VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
      viewportState.viewportCount = static_cast<uint32_t>(viewports.size());
      viewportState.pViewports = viewports.data();
      viewportState.scissorCount = static_cast<uint32_t>(scissors.size());
      viewportState.pScissors = scissors.data();

      // Rasterizer
      VkPipelineRasterizationStateCreateInfo rasterizer = spec.getRasterizer();

      // Multisampling
      VkPipelineMultisampleStateCreateInfo multisampling =
          spec.getMultisampling();

      // Color blending
      std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments =
          spec.getColorBlending();

      // Color blend state (depends on colorBlendAttachments)
      VkPipelineColorBlendStateCreateInfo colorBlending = {};
      colorBlending.sType =
          VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
      colorBlending.logicOpEnable = VK_FALSE;
      colorBlending.attachmentCount =
          static_cast<uint32_t>(colorBlendAttachments.size());
      colorBlending.pAttachments = colorBlendAttachments.data();

      // Descriptor set layouts
      std::vector<VkDescriptorSetLayout> descriptorSetLayouts =
          spec.getDescriptorSetLayouts();

      // Pipeline layout (depends on descriptorSetLayouts)
      VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
      pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
      pipelineLayoutInfo.setLayoutCount =
          static_cast<uint32_t>(descriptorSetLayouts.size());
      pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();

      // Depth/Stencil
      VkPipelineDepthStencilStateCreateInfo depthStencil =
          spec.getDepthStencil();

      // Shader stages
      std::vector<VkPipelineShaderStageCreateInfo> shaderStages{shaders.size()};
      for (const auto &shader : shaders) {
        shaderStages.push_back(shader.getInfo());
      }

      // Final structure (depends on all of the above + render pass)
      VkGraphicsPipelineCreateInfo pipelineInfo = {};
      pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
      pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
      pipelineInfo.pStages = shaderStages.data();
      pipelineInfo.pVertexInputState = &vertexInputInfo;
      pipelineInfo.pInputAssemblyState = &inputAssembly;
      pipelineInfo.pViewportState = &viewportState;
      pipelineInfo.pRasterizationState = &rasterizer;
      pipelineInfo.pMultisampleState = &multisampling;
      pipelineInfo.pDepthStencilState = &depthStencil;
      pipelineInfo.pColorBlendState = &colorBlending;
      pipelineInfo.pDynamicState = nullptr;
      pipelineInfo.renderPass = renderPass;
      pipelineInfo.subpass = 0;
      pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
      pipelineInfo.basePipelineIndex = -1;

      // Try to create the pipeline and return a VkResult
      return pipeline.create(pipelineLayoutInfo, pipelineInfo);
    }
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_PIPELINE_FACTORY_HPP__