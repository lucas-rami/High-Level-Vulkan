#ifndef __HL_VULKAN_SHADER_HPP__
#define __HL_VULKAN_SHADER_HPP__

#include "device.hpp"
#include "hl_vulkan.hpp"

#include <optional>

namespace HLVulkan {

  class Shader {

  public:
    static int readFile(const std::string &filename, std::vector<char> &data);

    Shader(VkDevice device, const std::string &filename,
           VkShaderStageFlagBits stage, const std::string & = "main");

    std::optional<VkPipelineShaderStageCreateInfo> getShaderStageInfo();

    virtual ~Shader();

  private:
    VkDevice device;
    std::string filename;
    std::string pName;
    VkShaderStageFlagBits stage;
    VkShaderModule shaderModule = VK_NULL_HANDLE;
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_SHADER_HPP__