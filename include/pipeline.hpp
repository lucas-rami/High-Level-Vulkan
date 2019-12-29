#ifndef __HL_VULKAN_PIPELINE_HPP__
#define __HL_VULKAN_PIPELINE_HPP__

#include "device.hpp"
#include "pipeline_spec.hpp"
#include "shader.hpp"
#include "vertex_spec.hpp"

namespace HLVulkan {

  /**
   * @brief Packages a VkPipeline and VkPipelineLayout.
   */
  class Pipeline {
  public:
    /**
     * @brief Creates a pipeline following some vertex and pipeline
     * specification.
     *
     * @param[in] device The device to allocate the pipeline from.
     * @param[in] vertSpec A vertex specification.
     * @param[in] pipeSpec A pipeline specification.
     * @param[in] shaders The set of shaders involved in the pipeline.
     * @param[in] renderPass The render pass associated to the pipeline.
     *
     * @throw std::runtime_error If pipeline creation fails for some reason.
     */
    Pipeline(const Device &device, const VertexSpec &vertSpec,
             const PipelineSpec &pipeSpec, const std::vector<Shader> &shaders,
             VkRenderPass renderPass);

    /**
     * @brief Deleted copy-constructor.
     */
    Pipeline(const Pipeline &other) = delete;

    /**
     * @brief Deleted copy-assignment operator.
     */
    Pipeline &operator=(const Pipeline &other) = delete;

    /**
     * @brief Move-constructor.
     */
    Pipeline(Pipeline &&other);

    /**
     * @brief Move-assignment operator.
     */
    Pipeline &operator=(Pipeline &&other);

    /**
     * @brief Destructor.
     */
    virtual ~Pipeline();

  private:
    // The device to allocate the pipeline from
    VkDevice device;
    // The pipeline handle
    VkPipeline pipeline{VK_NULL_HANDLE};
    // The pipeline layout handle
    VkPipelineLayout layout{VK_NULL_HANDLE};
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_PIPELINE_HPP__