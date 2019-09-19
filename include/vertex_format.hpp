#ifndef __HL_VULKAN_VERTEX_FORMAT_HPP__
#define __HL_VULKAN_VERTEX_FORMAT_HPP__

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vulkan/vulkan.h>

#include <vector>

namespace HLVulkan {

    class VertexFormat {

      public:
        std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() const;
        VkVertexInputBindingDescription getBindingDescription() const;

        virtual ~VertexFormat();

      private:
        virtual std::vector<VkVertexInputAttributeDescription> createAttributeDescriptions() const = 0;
        virtual VkVertexInputBindingDescription createBindingDescription() const = 0;
    };

} // namespace HLVulkan

#endif //__HL_VULKAN_VERTEX_FORMAT_HPP__