#ifndef __HL_VULKAN_SHADER_HPP__
#define __HL_VULKAN_SHADER_HPP__

#include "device.hpp"
#include "hl_vulkan.hpp"

#include <optional>

namespace HLVulkan {

  class Shader {

  public:
    Shader(VkDevice device, const std::string &filename,
           VkShaderStageFlagBits stage, const std::string &pName = "main");

    Shader(VkDevice device, const std::vector<char> &code,
           VkShaderStageFlagBits stage, const std::string &pName = "main");

    Shader(const Shader &other) = delete;

    Shader &operator=(const Shader &other) = delete;

    Shader(Shader &&other);

    Shader &operator=(Shader &&other);

    VkPipelineShaderStageCreateInfo getInfo() const;

    static int shaderFromFile(const std::string &filename,
                              std::vector<char> &code);

    static VkResult createShaderModule(VkDevice device,
                                       const std::vector<char> &code,
                                       VkShaderModule &shaderModule);

    virtual ~Shader();

  private:
    VkDevice device = VK_NULL_HANDLE;
    VkShaderModule handle = VK_NULL_HANDLE;
    VkPipelineShaderStageCreateInfo shaderInfo = {};

    void createShaderInfo(VkShaderStageFlagBits stage,
                          const std::string &pName);
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_SHADER_HPP__