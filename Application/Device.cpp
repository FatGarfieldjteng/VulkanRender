#include "Device.h"

#include <stdexcept>
#include <iostream>
#include <vector>

Device::Device()
{

}

Device::~Device()
{
    vkDestroyDevice(mLogicalDevice, nullptr);
}

void Device::create(VkInstance VkInstance)
{
    createPhysicalDevice(VkInstance);
    createLogicalDevice();
}

void Device::acquireQueue(VkQueue* graphicsQueue)
{
    vkGetDeviceQueue(mLogicalDevice, mQueueFamilyIndices.graphicsFamily.value(), 0, graphicsQueue);
}

void Device::createPhysicalDevice(VkInstance VkInstance)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(VkInstance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(VkInstance, &deviceCount, physicalDevices.data());

    for (const auto& physicalDevice : physicalDevices) {
        if (isDeviceSuitable(physicalDevice)) {
            mPhysicalDevice = physicalDevice;
            break;
        }
    }

    if (mPhysicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

void Device::createLogicalDevice()
{
    mQueueFamilyIndices = findQueueFamilies(mPhysicalDevice);

    // queue creation information
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = mQueueFamilyIndices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;

    // set priority of the queue
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    // no specific feature needed
    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = 0;

    if (vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mLogicalDevice) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }
}

bool Device::isDeviceSuitable(VkPhysicalDevice physicalDevice)
{
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
        
    return (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && indices.isComplete());
}

Device::QueueFamilyIndices Device::findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    for (size_t i = 0; i < queueFamilies.size(); ++i)
    {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = static_cast<uint32_t>(i);
            break;
        }
    }

    return indices;
}