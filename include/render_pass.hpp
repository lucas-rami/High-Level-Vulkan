#ifndef __HL_VULKAN_RENDER_PASS_HPP__
#define __HL_VULKAN_RENDER_PASS_HPP__

#include "device.hpp"
#include "render_pass_spec.hpp"

namespace HLVulkan {

  /**
   * @brief Packages a VkRenderPass.
   */
  class RenderPass {
  public:
    /**
     * @brief Creates a render pass following some render pass specification.
     *
     * @param[in] device The device to allocate the render pass from.
     * @param[in] spec A render pass specification.
     *
     * @throw std::runtime_error If render pass creation fails for some reason.
     */
    RenderPass(const Device &device, const RenderPassSpec &spec);

    /**
     * @brief Deleted copy-constructor.
     */
    RenderPass(const RenderPass &other) = delete;

    /**
     * @brief Deleted copy-assignment operator.
     */
    RenderPass &operator=(const RenderPass &other) = delete;

    /**
     * @brief Move-constructor.
     */
    RenderPass(RenderPass &&other);

    /**
     * @brief Move-assignment operator.
     */
    RenderPass &operator=(RenderPass &&other);

    /**
     * @brief Destructor.
     */
    virtual ~RenderPass();

  private:
    VkDevice device;
    VkRenderPass renderPass{VK_NULL_HANDLE};
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_RENDER_PASS_HPP__