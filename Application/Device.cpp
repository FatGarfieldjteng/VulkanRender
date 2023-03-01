#include "Device.h"
#include "Validation.h"
#include "SwapChain.h"
#include "ShaderManager.h"
#include "Pipeline.h"
#include "PassManager.h"

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
    if (mPipeline)
    {
        delete mPipeline;
    }

    if (mShaderManager)
    {
        delete mShaderManager;
    }

    if (mSwapChain)
    {
        delete mSwapChain;
    }

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
    createSwapChain();
    createShaderManager();
    createPipelineLayout();
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
    mWindow = window;

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
    mQueueFamilyIndices = Queue::findQueueFamilies(mPhysicalDevice, mSurface);

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

void Device::createSwapChain()
{
    mSwapChain = new SwapChain();
    mSwapChain->create(mPhysicalDevice, 
        mLogicalDevice, 
        mWindow, 
        mSurface);
}

void Device::createShaderManager()
{
    mShaderManager = new ShaderManager(mLogicalDevice);
}

void Device::createPipelineLayout()
{
    mPipeline = new Pipeline();
    mPipeline->create(mLogicalDevice, mShaderManager);
}

void Device::createPassManager()
{
    mPassManager = new PassManager(mLogicalDevice, mSwapChain);
}

bool Device::isDeviceSuitable(VkPhysicalDevice physicalDevice)
{
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

    Queue::QueueFamilyIndices indices = Queue::findQueueFamilies(physicalDevice, mSurface);

    bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChain::SwapChainSupportDetails swapChainSupport = SwapChain::querySwapChainSupport(physicalDevice, mSurface);
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