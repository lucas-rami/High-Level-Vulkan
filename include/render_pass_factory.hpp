#ifndef __HL_VULKAN_RENDER_PASS_FACTORY_HPP__
#define __HL_VULKAN_RENDER_PASS_FACTORY_HPP__

#include <unordered_set>
#include <vector>

#include "device.hpp"
#include "hl_vulkan.hpp"

namespace HLVulkan {

    class RenderPassFactory {

      public:
        RenderPassFactory(HLVulkan::Device &device);

        template <class RenderPassSpec> VkRenderPass generateNewRenderPass(const RenderPassSpec &spec) {
            VkRenderPass renderPass = createRenderPass(device, spec);
            if (renderPass != VK_NULL_HANDLE) {
                addRenderPassToSet(renderPass);
            }
            return renderPass;
        }

        template <class RenderPassSpec> static VkRenderPass createRenderPass(const HLVulkan::Device &device, const RenderPassSpec &spec) {

            auto attachments = spec.getAttachments();
            auto subpasses = spec.getSubpasses();
            auto dependencies = spec.getDependencies();

            VkRenderPassCreateInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
            renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            renderPassInfo.pAttachments = attachments.data();
            renderPassInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
            renderPassInfo.pSubpasses = subpasses.data();
            renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
            renderPassInfo.pDependencies = dependencies.data();

            // Create the render pass
            VkRenderPass renderPass;
            VK_CHECK_RET_NULL(vkCreateRenderPass(device.logical, &renderPassInfo, nullptr, &renderPass));

            // The render pass has been created successfully
            return renderPass;
        }

        void destroyRenderPass(VkRenderPass renderPass);

        virtual ~RenderPassFactory();

      private:
        struct RenderPassHasher {
            size_t operator()(const VkRenderPass &renderPass) const { return std::hash<VkRenderPass>()(renderPass); }
        };
        struct RenderPassComparator {
            bool operator()(const VkRenderPass &renderPass1, const VkRenderPass &renderPass2) const { return renderPass1 == renderPass2; }
        };

        HLVulkan::Device device;
        std::unordered_set<VkRenderPass, RenderPassHasher, RenderPassComparator> createdRenderPasses;

        void addRenderPassToSet(VkRenderPass &renderPass);
    };

} // namespace HLVulkan

#endif //__HL_VULKAN_RENDER_PASS_FACTORY_HPP__