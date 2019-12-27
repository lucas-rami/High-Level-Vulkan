#include "shader.hpp"

#include <fstream>

namespace HLVulkan {

  Shader::Shader(const Device &device, const std::string &filename,
                 VkShaderStageFlagBits stage, const std::string &pName)
      : device(*device) {

    std::vector<char> code;
    ASSERT_THROW(shaderFromFile(filename, code), "failed to read shader file");
    VK_THROW(createShaderModule(code), "failed to create shader module");

    // Create shader info
    createShaderInfo(stage, pName);
  }

  Shader::Shader(const Device &device, const std::vector<char> &code,
                 VkShaderStageFlagBits stage, const std::string &pName)
      : device(*device) {

    VK_THROW(createShaderModule(code), "failed to create shader module");

    // Create shader info
    createShaderInfo(stage, pName);
  }

  VkPipelineShaderStageCreateInfo Shader::getInfo() const { return shaderInfo; }

  Shader::Shader(Shader &&other)
      : device(other.device), module(other.module),
        shaderInfo(other.shaderInfo) {
    other.module = VK_NULL_HANDLE;
  }

  Shader &Shader::operator=(Shader &&other) {
    // Self-assignment detection
    if (&other != this) {
      device = other.device;
      module = other.module;
      shaderInfo = other.shaderInfo;

      other.module = VK_NULL_HANDLE;
    }
    return *this;
  }

  Shader::~Shader() {
    if (module != VK_NULL_HANDLE) {
      vkDestroyShaderModule(device, module, nullptr);
    }
  }

  uint32_t Shader::shaderFromFile(const std::string &filename,
                                  std::vector<char> &code) {
    // Open the file in read binary mode
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
      return 1;
    }

    // Store the file's content in the data buffer
    size_t fileSize = (size_t)file.tellg();
    code.resize(fileSize);
    file.seekg(0);
    file.read(code.data(), fileSize);
    if (code.size() != fileSize) {
      return 1;
    }

    // Close the file and return
    file.close();
    return 0;
  }

  VkResult Shader::createShaderModule(const std::vector<char> &code) {

    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    return vkCreateShaderModule(device, &createInfo, nullptr, &module);
  }

  void Shader::createShaderInfo(VkShaderStageFlagBits stage,
                                const std::string &pName) {
    shaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderInfo.stage = stage;
    shaderInfo.module = module;
    shaderInfo.pName = pName.c_str();
  }

} // namespace HLVulkan
