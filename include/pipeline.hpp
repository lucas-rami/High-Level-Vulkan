#ifndef __HL_VULKAN_PIPELINE_HPP__
#define __HL_VULKAN_PIPELINE_HPP__

#include "hl_vulkan.hpp"

namespace HLVulkan {

  class Pipeline {
  public:
    Pipeline(VkDevice device, VkPipeline handle, VkPipelineLayout layout);

    Pipeline(const Pipeline &other) = delete;
    
    Pipeline& operator=(const Pipeline &other) = delete;

    Pipeline(Pipeline &&other) = delete;
    
    Pipeline& operator=(Pipeline &&other) = delete;

    virtual ~Pipeline();

  private:
    VkDevice device = VK_NULL_HANDLE;
    VkPipeline handle = VK_NULL_HANDLE;
    VkPipelineLayout layout = VK_NULL_HANDLE;
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_PIPELINE_HPP__