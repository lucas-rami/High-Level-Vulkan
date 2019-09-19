#include "vertex_format.hpp"

namespace HLVulkan {

    std::vector<VkVertexInputAttributeDescription> VertexFormat::getAttributeDescriptions() const { return createAttributeDescriptions(); }
    VkVertexInputBindingDescription VertexFormat::getBindingDescription() const { return createBindingDescription(); }

    VertexFormat::~VertexFormat() {}

} // namespace HLVulkan