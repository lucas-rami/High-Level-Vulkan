#ifndef __HL_VULKAN_SHADER_HPP__
#define __HL_VULKAN_SHADER_HPP__

#include "device.hpp"
#include "hl_vulkan.hpp"

#include <optional>

namespace HLVulkan {

  /**
   * @brief Packages a VkShaderModule.
   */
  class Shader {

  public:
    /**
     * @brief Creates a shader from the data stored in a file.
     *
     * @param[in] device The device to allocate the shader module from.
     * @param[in] filename The file containing the compiled shader code.
     * @param[in] stage The shader's stage.
     * @param[in] pNmae Name of the shader's entrypoint.
     *
     * @throw std::runtime_error If shader creation fails for some reason.
     */
    Shader(const Device &device, const std::string &filename,
           VkShaderStageFlagBits stage, const std::string &pName = "main");

    /**
     * @brief Creates a shader from binary data.
     *
     * @param[in] device The device to allocate the shader module from.
     * @param[in] code Compiled shader code.
     * @param[in] stage The shader's stage.
     * @param[in] pNmae Name of the shader's entrypoint.
     *
     * @throw std::runtime_error If shader creation fails for some reason.
     */
    Shader(const Device &device, const std::vector<char> &code,
           VkShaderStageFlagBits stage, const std::string &pName = "main");

    /**
     * @brief Get the shader module info.
     *
     * @return The shader module info device.
     */
    VkPipelineShaderStageCreateInfo getInfo() const;

    /**
     * @brief Deleted copy-constructor.
     */
    Shader(const Shader &other) = delete;

    /**
     * @brief Deleted copy-assignment operator.
     */
    Shader &operator=(const Shader &other) = delete;

    /**
     * @brief Move-constructor.
     */
    Shader(Shader &&other);

    /**
     * @brief Move-assignment operator.
     */
    Shader &operator=(Shader &&other);

    /**
     * @brief Destructor.
     */
    virtual ~Shader();

  private:
    // The device the shader is allocated from
    VkDevice device{VK_NULL_HANDLE};
    // The shader module
    VkShaderModule module{VK_NULL_HANDLE};
    // The shader info structure
    VkPipelineShaderStageCreateInfo shaderInfo{};

    /**
     * @brief Reads compiled shader code from a file.
     *
     * @param[in] filename The file.
     * @param[out] code A vector filled with the file's content on success.
     *
     * @return 0 on success, 1 on error.
     */
    uint32_t shaderFromFile(const std::string &filename,
                            std::vector<char> &code);

    /**
     * @brief Creates the shader module.
     *
     * @param[in] code The compiled shader code.
     *
     * @return A Vulkan error code if module creation fails, VK_SUCCESS
     * otherwise.
     */
    VkResult createShaderModule(const std::vector<char> &code);

    /**
     * @brief Fills in the shader info structure.
     *
     * @param[in] stage The shader's stage.
     * @param[in] pNmae Name of the shader's entrypoint.
     */
    void createShaderInfo(VkShaderStageFlagBits stage,
                          const std::string &pName);
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_SHADER_HPP__