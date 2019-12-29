#include "vertex_spec.hpp"

namespace HLVulkan {

  void VertexSpec::getAttributeDescriptions(
      std::vector<VkVertexInputAttributeDescription> &attributes) const {
    createAttributeDescriptions(attributes);
  }
  void VertexSpec::getBindingDescriptions(
      std::vector<VkVertexInputBindingDescription> &bindings) const {
    createBindingDescriptions(bindings);
  }

  VertexSpec::~VertexSpec() {}

} // namespace HLVulkan
