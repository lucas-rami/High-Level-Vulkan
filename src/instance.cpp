#include "instance.hpp"
#include "hl_vulkan.hpp"

#include <GLFW/glfw3.h>
#include <string.h>

namespace HLVulkan {

  std::vector<const char *> glfwRequiredExt() {
    uint32_t glfwExtCount = 0;
    const char **glfwExt = glfwGetRequiredInstanceExtensions(&glfwExtCount);
    std::vector<const char *> extensions(glfwExt, glfwExt + glfwExtCount);
    return extensions;
  }

  bool glfwAreExtSupported(std::vector<const char *> &glfwExt) {

    // Get list of extensions supported by Vulkan
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                           extensions.data());

    // Check that every extension is supported by Vulkan
    for (const auto &glfwExtension : glfwExt) {
      bool isSupported = false;
      for (const auto &vulkanExtension : extensions) {
        if (strcmp(glfwExtension, vulkanExtension.extensionName) == 0) {
          isSupported = true;
          break;
        }
      }
      if (!isSupported) {
        return false;
      }
    }
    return true;
  }

  Instance::Instance(const std::string &appName) {

    // Ask GLFW for extensions to interface with the window system
    // and check for support
    std::vector<const char *> glfwExt = glfwRequiredExt();
    ASSERT_FAIL(glfwAreExtSupported(glfwExt), "glfw extensions not supported");

    // Create the instance
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = appName.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "HLVulkan";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExt.size());
    createInfo.ppEnabledExtensionNames = glfwExt.data();

    // Create the Vulkan instance
    VK_THROW(vkCreateInstance(&createInfo, nullptr, &instance),
             "failed to create instance");
  }

  VkInstance Instance::operator*() const { return instance; }

  Instance::Instance(Instance &&other) : instance(other.instance) {
    other.instance = VK_NULL_HANDLE;
  }

  Instance &Instance::operator=(Instance &&other) {
    // Self-assignment detection
    if (&other != this) {
      instance = other.instance;
      other.instance = VK_NULL_HANDLE;
    }
    return *this;
  }

  Instance::~Instance() {
    if (instance != VK_NULL_HANDLE) {
      vkDestroyInstance(instance, nullptr);
    }
  }

} // namespace HLVulkan