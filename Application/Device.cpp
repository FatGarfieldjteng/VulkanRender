#include "Device.h"
#include "Validation.h"
#include "SwapChain.h"

#include <stdexcept>
#include <iostream>
#include <set>

Device::Device(bool enableValidationLayers, Validation* validation)
    :mEnableValidationLayers(enableValidationLayers)
    ,mValidation(validation)
{

}

Device::~Device()
{
    if (mGraphicsQueue)
    {
        delete mGraphicsQueue;
    }
    if (mPresentQueue)
    {
        delete mPresentQueue;
    }

    vkDestroyDevice(mLogicalDevice, nullptr);
    vkDestroySurfaceKHR(mVkInstance, mSurface, nullptr);
}

void Device::create(VkInstance instance, GLFWwindow* window)
{
    mVkInstance = instance;
    createSurface(window);
    createPhysicalDevice();
    createLogicalDevice();
}

void Device::acquireQueue(Queue::Type type, VkQueue* queue)
{

    switch (type)
    {
    case Queue::Type::Graphics:
    {
        vkGetDeviceQueue(mLogicalDevice, 
            mQueueFamilyIndices.graphicsFamily.value(), 
            0, 
            queue);
    }
    break;

    case Queue::Type::Present:
    {
        vkGetDeviceQueue(mLogicalDevice, 
            mQueueFamilyIndices.presentFamily.value(), 
            0, 
            queue);
    }
    break;

    }
}

void Device::createSurface(GLFWwindow* window)
{
    if (glfwCreateWindowSurface(mVkInstance,
        window,
        nullptr,
        &mSurface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }
}

void Device::createPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(mVkInstance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
    vkEnumeratePhysicalDevices(mVkInstance, &deviceCount, physicalDevices.data());

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

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { mQueueFamilyIndices.graphicsFamily.value(), mQueueFamilyIndices.presentFamily.value() };

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(mSwapChainExtensionName.size());
    createInfo.ppEnabledExtensionNames = mSwapChainExtensionName.data();

    if (mEnableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(mValidation->getValidationLayersSize());
        createInfo.ppEnabledLayerNames = mValidation->getValidationLayersData();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mLogicalDevice) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    
    mGraphicsQueue = new Queue(Queue::Type::Graphics);
    mPresentQueue = new Queue(Queue::Type::Present);

    mGraphicsQueue->acquireQueue(this);
    mPresentQueue->acquireQueue(this);
}

bool Device::isDeviceSuitable(VkPhysicalDevice physicalDevice)
{
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

    Queue::QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChain::SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }
        
    return (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU 
        && indices.isComplete() 
        && extensionsSupported
        && swapChainAdequate);
}

bool Device::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(mSwapChainExtensionName.begin(), mSwapChainExtensionName.end());

    for (const auto& extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

Queue::QueueFamilyIndices Device::findQueueFamilies(VkPhysicalDevice device)
{
    Queue::QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    for (size_t i = 0; i < queueFamilies.size(); ++i)
    {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = static_cast<uint32_t>(i);
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, mSurface, &presentSupport);

        if (presentSupport) {
            indices.presentFamily = (uint32_t)i;
        }

        if (indices.isComplete()) {
            break;
        }

    }

    return indices;
}

SwapChain::SwapChainSupportDetails Device::querySwapChainSupport(VkPhysicalDevice device)
{
    SwapChain::SwapChainSupportDetails details;


    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, mSurface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentModeCount, details.presentModes.data());
    }

    return details;
}