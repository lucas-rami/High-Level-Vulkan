#ifndef __HL_VULKAN_RENDER_PASS_SPEC_HPP__
#define __HL_VULKAN_RENDER_PASS_SPEC_HPP__

#include <vulkan/vulkan.h>

#include <vector>

#include "hl_vulkan.hpp"

namespace HLVulkan {

    class RenderPassSpec {

      public:
        std::vector<VkAttachmentDescription> getAttachments() const;
        std::vector<VkSubpassDescription> getSubpasses() const;
        std::vector<VkSubpassDependency> getDependencies() const;

        virtual ~RenderPassSpec();

      private:
        virtual std::vector<VkAttachmentDescription> createAttachments() const = 0;
        virtual std::vector<VkSubpassDescription> createSubpasses() const = 0;
        virtual std::vector<VkSubpassDependency> createDependencies() const = 0;
    };

} // namespace HLVulkan

#endif //__HL_VULKAN_RENDER_PASS_SPEC_HPP__