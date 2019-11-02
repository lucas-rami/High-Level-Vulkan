#include "pipeline.hpp"

namespace HLVulkan {

  Pipeline::Pipeline(VkDevice device) : device(device) {}

  Pipeline::Pipeline(Pipeline &&other)
      : device(other.device), handle(other.handle), layout(other.layout) {
    other.handle = VK_NULL_HANDLE;
  }

  Pipeline &Pipeline::operator=(Pipeline &&other) {
    // Self-assignment detection
    if (&other != this) {
      device = other.device;
      handle = other.handle;
      layout = other.layout;

      other.handle = VK_NULL_HANDLE;
    }
    return *this;
  };

  VkResult Pipeline::create(const VkPipelineLayoutCreateInfo &layoutInfo,
                            VkGraphicsPipelineCreateInfo &pipelineInfo) {

    ASSERT_FAIL(handle == VK_NULL_HANDLE, "pipeline already created");

    // Create layout
    VK_RET(vkCreatePipelineLayout(device, &layoutInfo, nullptr, &layout));

    // Update layout in pipeline creation structure
    pipelineInfo.layout = layout;

    // Create pipeline
    VkResult ret;
    if ((ret = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1,
                                         &pipelineInfo, nullptr, &handle)) !=
        VK_SUCCESS) {
      vkDestroyPipelineLayout(device, layout, nullptr);
      return ret;
    }

    return VK_SUCCESS;
  }

  VkDevice Pipeline::getDevice() const { return device; }

  Pipeline::~Pipeline() {
    if (handle != VK_NULL_HANDLE) {
      vkDestroyPipeline(device, handle, nullptr);
      vkDestroyPipelineLayout(device, layout, nullptr);
    }
  }

} // namespace HLVulkan
