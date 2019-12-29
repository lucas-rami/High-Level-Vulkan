#include "pipeline.hpp"

namespace HLVulkan {

  Pipeline::Pipeline(const Device &device, const VertexSpec &vertSpec,
                     const PipelineSpec &pipeSpec,
                     const std::vector<Shader> &shaders,
                     VkRenderPass renderPass)
      : device(*device) {

    // ***** Create pipeline layout *****

    // Descriptor set layouts
    std::vector<VkDescriptorSetLayout> descSetLayouts;
    pipeSpec.getDescriptorSetLayouts(descSetLayouts);

    // Pipeline layout (depends on descSetLayouts)
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount =
        static_cast<uint32_t>(descSetLayouts.size());
    pipelineLayoutInfo.pSetLayouts = descSetLayouts.data();

    VK_THROW(vkCreatePipelineLayout(this->device, &pipelineLayoutInfo, nullptr,
                                    &layout),
             "failed to create layout");

    // ***** Create pipeline *****

    // Vertex specification
    std::vector<VkVertexInputAttributeDescription> attributes;
    vertSpec.getAttributeDescriptions(attributes);
    std::vector<VkVertexInputBindingDescription> bindings{};
    vertSpec.getBindingDescriptions(bindings);

    // Vertex input
    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType =
        VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount =
        static_cast<uint32_t>(bindings.size());
    vertexInputInfo.pVertexBindingDescriptions = bindings.data();
    vertexInputInfo.vertexAttributeDescriptionCount =
        static_cast<uint32_t>(attributes.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributes.data();

    // Input assembly
    VkPipelineInputAssemblyStateCreateInfo inputAssembly;
    pipeSpec.getInputAssembly(inputAssembly);

    // Viewports
    std::vector<VkViewport> viewports;
    pipeSpec.getViewports(viewports);

    // Scissors
    std::vector<VkRect2D> scissors;
    pipeSpec.getScissors(scissors);

    // Viewport state
    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = static_cast<uint32_t>(viewports.size());
    viewportState.pViewports = viewports.data();
    viewportState.scissorCount = static_cast<uint32_t>(scissors.size());
    viewportState.pScissors = scissors.data();

    // Rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizer;
    pipeSpec.getRasterizer(rasterizer);

    // Multisampling
    VkPipelineMultisampleStateCreateInfo multisampling;
    pipeSpec.getMultisampling(multisampling);

    // Color blending
    std::vector<VkPipelineColorBlendAttachmentState> colorBlends;
    pipeSpec.getColorBlending(colorBlends);

    // Color blend state (depends on colorBlends)
    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType =
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.attachmentCount = static_cast<uint32_t>(colorBlends.size());
    colorBlending.pAttachments = colorBlends.data();

    // Depth/Stencil
    VkPipelineDepthStencilStateCreateInfo depthStencil;
    pipeSpec.getDepthStencil(depthStencil);

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
    pipelineInfo.layout = layout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    // Create pipeline
    VkResult ret;
    if ((ret = vkCreateGraphicsPipelines(this->device, VK_NULL_HANDLE, 1,
                                         &pipelineInfo, nullptr, &pipeline)) !=
        VK_SUCCESS) {
      vkDestroyPipelineLayout(this->device, layout, nullptr);
      ASSERT_THROW(false, "failed to create pipeline")
    }
  }

  Pipeline::Pipeline(Pipeline &&other)
      : device(other.device), pipeline(other.pipeline), layout(other.layout) {
    other.pipeline = VK_NULL_HANDLE;
  }

  Pipeline &Pipeline::operator=(Pipeline &&other) {
    // Self-assignment detection
    if (&other != this) {
      device = other.device;
      pipeline = other.pipeline;
      layout = other.layout;

      other.pipeline = VK_NULL_HANDLE;
    }
    return *this;
  };

  Pipeline::~Pipeline() {
    if (pipeline != VK_NULL_HANDLE) {
      vkDestroyPipeline(device, pipeline, nullptr);
      vkDestroyPipelineLayout(device, layout, nullptr);
    }
  }

} // namespace HLVulkan
