#include "hl_vulkan.hpp"

#include <fstream>

namespace HLVulkan {

    static VkResult createShaderModule(VkDevice logical, const std::vector<char> &code, VkShaderModule &shaderModule) {

        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

        return vkCreateShaderModule(logical, &createInfo, nullptr, &shaderModule);
    }

    // @TODO define custom error codes
    int Shader::readFile(const std::string &filename, std::vector<char> &data) {
        // Open the file in read binary mode
        std::ifstream file(filename, std::ios::ate | std::ios::binary);
        if (!file.is_open()) {
            return 1;
        }

        // Store the file's content in the data buffer
        size_t fileSize = (size_t)file.tellg();
        data.resize(fileSize);
        file.seekg(0);
        file.read(data.data(), fileSize);

        if (data.size() != fileSize) {
            return 2;
        }

        // Close the file and return
        file.close();
        return 0;
    }

    Shader::Shader(Device device, const std::string &filename, VkShaderStageFlagBits stage, const std::string &pName)
        : device(device), filename(filename), stage(stage), pName(pName) {}

    std::optional<VkPipelineShaderStageCreateInfo> Shader::getShaderStageInfo() {

        if (shaderModule == VK_NULL_HANDLE) {
            std::vector<char> data;
            int ret;
            if ((ret = readFile(filename, data))) {
                std::cout << "failed to read shader " << filename << ": readfile() failed with error code " << ret << std::endl;
                return {};
            }

            VkResult vkRet;
            if ( (vkRet = createShaderModule(device.logical, data, shaderModule)) != VK_SUCCESS) {
                std::cout << "failed to create shader module: vkCreateShaderModule() failed with error code " << vkRet << std::endl;                
                return {};
            }
        }

        VkPipelineShaderStageCreateInfo shaderStageInfo = {};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.stage = stage;
        shaderStageInfo.module = shaderModule;
        shaderStageInfo.pName = pName.c_str();

        return shaderStageInfo;
    }

    Shader::~Shader() {
        if (shaderModule) {
            vkDestroyShaderModule(device.logical, shaderModule, nullptr);
        }
    }

} // namespace HLVulkan
