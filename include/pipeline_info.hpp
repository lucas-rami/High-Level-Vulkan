#ifndef __HL_VULKAN_PIPELINE_INFO_HPP__
#define __HL_VULKAN_PIPELINE_INFO_HPP__

#include "hl_vulkan.hpp"

namespace HLVulkan {

    // @TODO: doesn't handle its own destruction. It should do it
    struct PipelineInfo {
        const VkPipeline pipeline;
        const VkPipelineLayout layout;

        PipelineInfo(VkPipeline pipeline, VkPipelineLayout layout) : pipeline(pipeline), layout(layout) {}
        PipelineInfo(const PipelineInfo &info) : pipeline(info.pipeline), layout(info.layout) {}
    };

} // namespace HLVulkan

#endif //__HL_VULKAN_PIPELINE_INFO_HPP__