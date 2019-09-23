#include "hl_vulkan.hpp"

namespace HLVulkan {

  bool hasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
           format == VK_FORMAT_D24_UNORM_S8_UINT;
  }

} // namespace HLVulkan
