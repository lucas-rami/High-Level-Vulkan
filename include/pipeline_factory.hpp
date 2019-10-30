#ifndef __HL_VULKAN_PIPELINE_FACTORY_HPP__
#define __HL_VULKAN_PIPELINE_FACTORY_HPP__

#include <unordered_set>

#include "pipeline.hpp"
#include "pipeline_spec.hpp"
#include "shader.hpp"

namespace HLVulkan {

  class PipelineFactory {

  public:
    template <class VertexFormat, class PipelineSpec>
    static PipelineInfo
    createGraphicPipeline(VkDevice device, const VertexFormat &vertFormat,
                          const PipelineSpec &spec, VkRenderPass renderPass) {

      // Load 2 dummy shaders (vertex + fragment). ShaderModules freed
      // automatically when these objects go out of scope
      HLVulkan::Shader vertexShader{device, "../data/shaders/vk_vert.spv",
                                    VK_SHADER_STAGE_VERTEX_BIT};
      HLVulkan::Shader fragmentShader{device, "../data/shaders/vk_frag.spv",
                                      VK_SHADER_STAGE_FRAGMENT_BIT};

      // Create shader stages info
      auto vertexShaderInfo = vertexShader.getShaderStageInfo();
      auto fragmentShaderInfo = fragmentShader.getShaderStageInfo();
      if (!vertexShaderInfo || !fragmentShaderInfo) {
        return {VK_NULL_HANDLE, VK_NULL_HANDLE};
      }

      std::vector<VkPipelineShaderStageCreateInfo> shaderStages{
          *vertexShaderInfo, *fragmentShaderInfo};

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

      VkResult ret;
      VkPipelineLayout layout;
      if ((ret = vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr,
                                        &layout)) != VK_SUCCESS) {
        return {VK_NULL_HANDLE, VK_NULL_HANDLE};
      }

      // Depth/Stencil
      VkPipelineDepthStencilStateCreateInfo depthStencil =
          spec.getDepthStencil();

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
      pipelineInfo.layout = layout;
      pipelineInfo.renderPass = renderPass;
      pipelineInfo.subpass = 0;
      pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
      pipelineInfo.basePipelineIndex = -1;

      // Create the graphics pipeline
      VkPipeline pipeline;
      if ((ret = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1,
                                           &pipelineInfo, nullptr,
                                           &pipeline)) != VK_SUCCESS) {
        vkDestroyPipelineLayout(device.logical, layout, nullptr);
        return {VK_NULL_HANDLE, VK_NULL_HANDLE};
      }

      // The pipeline has been created successfully
      return {pipeline, layout};
    }

    virtual ~PipelineFactory();
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_PIPELINE_FACTORY_HPP__