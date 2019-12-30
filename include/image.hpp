#ifndef __HL_VULKAN_IMAGE_HPP__
#define __HL_VULKAN_IMAGE_HPP__

#include "buffer.hpp"
#include "command_pool.hpp"
#include "device.hpp"

namespace HLVulkan {

  /**
   * @brief Packages a VkImage together with a VkDeviceMemory and VkImageView.
   */
  class Image {

  public:
    /**
     * @brief Creates an image, allocates memory for it, binds the image to its
     * memory, and creates an image view.
     *
     * @param[in] device The device to allocate the image from.
     * @param[in] extent The image size.
     * @param[in] format The image format.
     * @param[in] tiling The image tiling mode.
     * @param[in] usage Usage flags for the image.
     * @param[in] aspect Aspect flags for the image view.
     * @param[in] properties Property flags for the image memory.
     *
     * @throw std::runtime_error If image creation fails for some reason.
     */
    Image(const Device &device, VkExtent2D extent, VkFormat format,
          VkImageTiling tiling, VkImageUsageFlags usage,
          VkImageAspectFlags aspect, VkMemoryPropertyFlags properties);

    /**
     * @brief Copies the image's content to another image.
     *
     * @param[in] dstImage The destination image.
     * @param[in] commandPool The command pool to use to submit the memory copy
     * command.
     *
     * @return A Vulkan error code if a call to the Vulkan API fails, VK_SUCCESS
     * otherwise.
     */
    VkResult copyTo(const Image &dstImage, const CommandPool &commandPool);

    /**
     * @brief Copies a buffer's content to the image.
     *
     * @param[in] layout The image's current layout.
     * @param[in] buffer The buffer to copy content from.
     * @param[in] commandPool The command pool to use to submit the memory copy
     * command.
     *
     * @return A Vulkan error code if a call to the Vulkan API fails, VK_SUCCESS
     * otherwise.
     */
    VkResult copyFromBuffer(VkImageLayout layout, const Buffer &srcBuffer,
                            const CommandPool &commandPool);

    /**
     * @brief Transitions the image layout using a pipeline barrier.
     * 
     * @param[in] oldLayout The old image layout.
     * @param[in] newLayout The new image layout.
     * @param[in] commandPool The command pool to use to submit the pipeline
     * barrier command.
     * 
     * @return A Vulkan error code if a call to the Vulkan API fails, VK_SUCCESS
     * otherwise.
     */
    VkResult transitionImageLayout(VkImageLayout oldLayout,
                                   VkImageLayout newLayout,
                                   const CommandPool &commandPool);

    /**
     * @brief Get the Vulkan image.
     *
     * @return The packaged Vulkan image.
     */
    VkImage operator*() const;

    /**
     * @brief Get the image view.
     *
     * @return The image view.
     */
    VkImageView getView() const;

    /**
     * @brief Get the image memory.
     *
     * @return The image memory.
     */
    VkDeviceMemory getMemory() const;

    /**
     * @brief Deleted copy-constructor.
     */
    Image(Image &other) = delete;

    /**
     * @brief Deleted copy-assignment operator.
     */
    Image &operator=(Image &other) = delete;

    /**
     * @brief Move-constructor.
     */
    Image(Image &&other);

    /**
     * @brief Move-assignment operator.
     */
    Image &operator=(Image &&other);

    /**
     * @brief Destructor.
     */
    virtual ~Image();

    /**
     * @brief Creates an image view from an image.
     *
     * @param[in] device The device to create the view from.
     * @param[in] image The image to create the view from.
     * @param[in] format The view's format.
     * @param[in] aspect The view's aspect flags.
     * @param[out] imageView The image view handle
     *
     * @return A Vulkan error code if a code to the Vulkan API fails, VK_SUCCESS
     * otherwise.
     */
    static VkResult createView(VkDevice device, VkImage image, VkFormat format,
                               VkImageAspectFlags aspect,
                               VkImageView &imageView);

  private:
    // The device to allocate the image from
    VkDevice device;
    // The image's extent
    VkExtent2D extent;
    // The image format
    VkFormat format;

    // The image handle
    VkImage image{VK_NULL_HANDLE};
    // The image memory handle
    VkDeviceMemory memory{VK_NULL_HANDLE};
    // The image view handle
    VkImageView imageView{VK_NULL_HANDLE};
  };

} // namespace HLVulkan

#endif //__HL_VULKAN_IMAGE_HPP__