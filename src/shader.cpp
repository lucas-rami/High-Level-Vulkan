#include "shader.hpp"

#include <fstream>

namespace HLVulkan {

  Shader::Shader(VkDevice device, const std::string &filename,
                 VkShaderStageFlagBits stage, const std::string &pName)
      : device(device) {

    std::vector<char> code;
    ASSERT_THROW(Shader::shaderFromFile(filename, code),
                 "failed to read shader file");
    VK_THROW(Shader::createShaderModule(device, code, handle),
             "failed to create shader module");

    // Create shader info
    createShaderInfo(stage, pName);
  }

  Shader::Shader(VkDevice device, const std::vector<char> &code,
                 VkShaderStageFlagBits stage, const std::string &pName)
      : device(device) {

    VK_THROW(Shader::createShaderModule(device, code, handle),
             "failed to create shader module");

    // Create shader info
    createShaderInfo(stage, pName);
  }

  Shader::Shader(Shader &&other)
      : device(other.device), handle(other.handle),
        shaderInfo(other.shaderInfo) {
    other.handle = VK_NULL_HANDLE;
  }

  Shader &Shader::operator=(Shader &&other) {
    // Self-assignment detection
    if (&other != this) {
      device = other.device;
      handle = other.handle;
      shaderInfo = other.shaderInfo;

      other.handle = VK_NULL_HANDLE;
    }
    return *this;
  }

  VkResult Shader::createShaderModule(VkDevice device,
                                      const std::vector<char> &code,
                                      VkShaderModule &shader) {

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    return vkCreateShaderModule(device, &createInfo, nullptr, &shader);
  }

  VkPipelineShaderStageCreateInfo Shader::getInfo() const { return shaderInfo; }

  uint32_t Shader::shaderFromFile(const std::string &filename,
                             std::vector<char> &code) {
    // Open the file in read binary mode
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
      return -1;
    }

    // Store the file's content in the data buffer
    size_t fileSize = (size_t)file.tellg();
    code.resize(fileSize);
    file.seekg(0);
    file.read(code.data(), fileSize);
    if (code.size() != fileSize) {
      return -1;
    }

    // Close the file and return
    file.close();
    return 0;
  }

  void Shader::createShaderInfo(VkShaderStageFlagBits stage,
                                const std::string &pName) {
    shaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderInfo.stage = stage;
    shaderInfo.module = handle;
    shaderInfo.pName = pName.c_str();
  }

  Shader::~Shader() {
    if (handle != VK_NULL_HANDLE) {
      vkDestroyShaderModule(device, handle, nullptr);
    }
  }

} // namespace HLVulkan
