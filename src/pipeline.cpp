#include "pipeline.hpp"

namespace HLVulkan {

  Pipeline::Pipeline(VkDevice device, VkPipeline pipeline,
                     VkPipelineLayout layout)
      : device(device), handle(handle), layout(layout) {}

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

  Pipeline::~Pipeline() {
    if (handle != VK_NULL_HANDLE) {
      vkDestroyPipeline(device, handle, nullptr);
      vkDestroyPipelineLayout(device, layout, nullptr);
    }
  }

} // namespace HLVulkan
