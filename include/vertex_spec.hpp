#ifndef __HL_VULKAN_VERTEX_SPEC_HPP__
#define __HL_VULKAN_VERTEX_SPEC_HPP__

#include "hl_vulkan.hpp"

namespace HLVulkan {

  /**
   * @brief A vertex format specification. The virtual private methods of this
   * class define the content of the structures used to specify the vertex input
   * state during pipeline creation.
   */
  class VertexSpec {

  public:

    /**
     * @brief See VertexSpec::createAttributeDescriptions.
     */
    void getAttributeDescriptions(
        std::vector<VkVertexInputAttributeDescription> &attributes) const;

    /**
     * @brief See VertexSpec::createBindingDescriptions.
     */
    void getBindingDescriptions(
        std::vector<VkVertexInputBindingDescription> &bindings) const;

    /**
     * @brief Destructor.
     */
    virtual ~VertexSpec();

  private:
    /**
     * @brief Specifies the vertex attribute descriptions.
     *
     * @param[out] attributes The vertex attribute descriptions.
     */
    virtual void createAttributeDescriptions(
        std::vector<VkVertexInputAttributeDescription> &attributes) const = 0;

    /**
     * @brief Specifies the vertex binding descriptions.
     *
     * @param[out] bindings The vertex binding descriptions.
     */
    virtual void createBindingDescriptions(
        std::vector<VkVertexInputBindingDescription> &bindings) const = 0;
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_VERTEX_SPEC_HPP__