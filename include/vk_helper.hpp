#ifndef __VULKAN_HELPERS_HPP__
#define __VULKAN_HELPERS_HPP__

#include <assert.h>
#include <iostream>
#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

#define ASSERT_MSG(b, err_str)                                                                                                                                 \
    {                                                                                                                                                          \
        if (!(b)) {                                                                                                                                            \
            std::cout << "[FATAL]: " << err_str << "\n in " << __FILE__ << " at line " << __LINE__ << std::endl;                                               \
            assert(b);                                                                                                                                         \
        }                                                                                                                                                      \
    }

#define VK_CHECK_NULL(handle)                                                                                                                                  \
    {                                                                                                                                                          \
        if (handle != VK_NULL_HANDLE) {                                                                                                                        \
            std::cout << "[FATAL]: " << #handle << " isn't VK_NULL_HANDLE"                                                                                     \
                      << "\n in " << __FILE__ << " at line " << __LINE__ << std::endl;                                                                         \
            assert(handle == VK_NULL_HANDLE);                                                                                                                  \
        }                                                                                                                                                      \
    }

#define VK_CHECK_NOT_NULL(handle)                                                                                                                              \
    {                                                                                                                                                          \
        if (handle == VK_NULL_HANDLE) {                                                                                                                        \
            std::cout << "[FATAL]: " << #handle << " is VK_NULL_HANDLE"                                                                                        \
                      << "\n in " << __FILE__ << " at line " << __LINE__ << std::endl;                                                                         \
            assert(handle != VK_NULL_HANDLE);                                                                                                                  \
        }                                                                                                                                                      \
    }

#define VK_CHECK_FAIL(b, err_str)                                                                                                                              \
    {                                                                                                                                                          \
        VkResult res = (b);                                                                                                                                    \
        if (res != VK_SUCCESS) {                                                                                                                               \
            std::cout << "[FATAL]: " << err_str << "\n in " << __FILE__ << " at line " << __LINE__ << std::endl;                                               \
            assert(res == VK_SUCCESS);                                                                                                                         \
        }                                                                                                                                                      \
    }

#define VK_CHECK_RET(b)                                                                                                                                        \
    {                                                                                                                                                          \
        VkResult res = (b);                                                                                                                                    \
        if (res != VK_SUCCESS) {                                                                                                                               \
            return res;                                                                                                                                        \
        }                                                                                                                                                      \
    }

#define VK_CHECK_RET_NULL(b)                                                                                                                                   \
    {                                                                                                                                                          \
        VkResult res = (b);                                                                                                                                    \
        if (res != VK_SUCCESS) {                                                                                                                               \
            return VK_NULL_HANDLE;                                                                                                                             \
        }                                                                                                                                                      \
    }

namespace VKHelper {

    bool hasStencilComponent(VkFormat format);

    struct Device {
        const VkPhysicalDevice physical;
        const VkDevice logical;

        Device(VkPhysicalDevice physicalDevice, VkDevice device);
        Device(const Device &device);

        const std::optional<uint32_t> findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

        const VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

        const VkFormat findDepthFormat();
    };

    struct Queue {
        VkQueue queue;
        uint32_t family;

        Queue(VkQueue queue, uint32_t family) : queue(queue), family(family){};
        Queue(const Queue &queue) : queue(queue.queue), family(queue.family){};
    };

    class Shader {

      private:
        Device device;
        std::string filename;
        VkShaderStageFlagBits stage;
        const std::string &pName;

        VkShaderModule shaderModule = VK_NULL_HANDLE;

      public:
        static int readFile(const std::string &filename, std::vector<char> &data);

        Shader(Device device, const std::string &filename, VkShaderStageFlagBits stage, const std::string &pName = "main");

        std::optional<VkPipelineShaderStageCreateInfo> getShaderStageInfo();

        ~Shader();
    };

    class CommandPool {

      private:
        Device device;
        Queue queue;

        VkCommandPool pool = VK_NULL_HANDLE;
        std::vector<VkCommandBuffer> commandBuffers;
        VkCommandBuffer singleTimeCommandBuffer = VK_NULL_HANDLE;

      public:
        CommandPool(Device device, Queue queue);

        CommandPool(Device device, Queue queue, uint32_t count);

        VkResult allocateCommandBuffers(uint32_t count);

        VkCommandBuffer beginSingleTimeCommands();

        VkResult endSingleTimeCommands(VkCommandBuffer commandBuffer);

        const VkCommandBuffer getCommandBuffer(size_t index);

        ~CommandPool();
    };

    class Buffer {

      private:
        Device device;
        VkDeviceSize size;
        VkBufferUsageFlags usage;
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkMemoryPropertyFlags memProperties = 0;

        VkResult bind();

      public:
        static VkResult createBuffer(VkDevice device, VkDeviceSize size, VkBufferUsageFlags usage, VkBuffer &buffer);

        Buffer(Device device, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

        Buffer(Device device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

        VkResult allocateBuffer(VkDeviceSize size);

        const VkResult mapAndCopy(void *dataToCopy, size_t size);

        const VkResult copyTo(const Buffer &dstBuffer, CommandPool &commandPool);

        const VkBufferUsageFlags getUsageFlags();
        const VkBuffer getBuffer();

        ~Buffer();
    };

    class Image {

      private:
        Device device;
        VkExtent2D extent;
        VkFormat format;
        VkImageTiling tiling;
        VkImageUsageFlags usage;
        VkImageAspectFlags aspect;
        VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED;

        VkImage image = VK_NULL_HANDLE;
        VkImageView imageView = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkMemoryPropertyFlags memProperties = 0;

      public:
        static VkResult createImage(VkDevice device, VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImage &image,
                                    VkImageLayout initLayout = VK_IMAGE_LAYOUT_UNDEFINED);

        static VkResult createImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspect, VkImageView &imageView);

        Image(Device device, VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImageAspectFlags aspect,
              VkImageLayout initLayout = VK_IMAGE_LAYOUT_UNDEFINED);

        Image(Device device, VkExtent2D extent, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkImageAspectFlags aspect,
              VkMemoryPropertyFlags properties, VkImageLayout initLayout = VK_IMAGE_LAYOUT_UNDEFINED);

        VkResult bind(VkMemoryPropertyFlags properties);

        const VkResult copyTo(const Image &dstImage, CommandPool &commandPool);

        const VkResult copyFromBuffer(Buffer &buffer, CommandPool &commandPool);

        VkResult transitionImageLayout(VkImageLayout newLayout, CommandPool &commandPool);

        const VkImageView getView();

        ~Image();
    };

    class Fence {
      private:
        Device device;
        VkFence fence = VK_NULL_HANDLE;

    public:
        Fence(Device device);

        const VkFence getFence();
        
        ~Fence();
    };

} // namespace VKHelper

#endif //__VULKAN_HELPERS_H__