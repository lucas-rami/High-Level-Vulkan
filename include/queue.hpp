#ifndef __HL_VULKAN_QUEUE_HPP__
#define __HL_VULKAN_QUEUE_HPP__

#include "hl_vulkan.hpp"

namespace HLVulkan {

    struct Queue {
        VkQueue queue;
        uint32_t family;

        Queue(VkQueue queue, uint32_t family) : queue(queue), family(family){};
        Queue(const Queue &queue) : queue(queue.queue), family(queue.family){};
    };

} // namespace HLVulkan

#endif //__HL_VULKAN_QUEUE_HPP__