#ifndef __HL_VULKAN_SHADER_HPP__
#define __HL_VULKAN_SHADER_HPP__

#include "device.hpp"
#include "hl_vulkan.hpp"

#include <optional>

namespace HLVulkan {

    class Shader {

      private:
        Device device;
        std::string filename;
        VkShaderStageFlagBits stage;
        const std::string pName;

        VkShaderModule shaderModule = VK_NULL_HANDLE;

      public:
        static int readFile(const std::string &filename, std::vector<char> &data);

        Shader(Device device, const std::string &filename, VkShaderStageFlagBits stage, const std::string & = "main");

        std::optional<VkPipelineShaderStageCreateInfo> getShaderStageInfo();

        ~Shader();
    };

} // namespace HLVulkan

#endif //__HL_VULKAN_SHADER_HPP__