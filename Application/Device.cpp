#include "Device.h"
#include "Validation.h"
#include "SwapChain.h"
#include "ShaderManager.h"
#include "Pipeline.h"
#include "PassManager.h"
#include "Managers.h"
#include "FrameBuffer.h"
#include "Command.h"
#include "SyncObjectManager.h"
#include "ConstantBufferManager.h"
#include "SimpleScene.h"

#include <stdexcept>
#include <iostream>
#include <set>

Device::Device(bool enableValidationLayers, 
    Validation* validation,
    unsigned int maxFramesInFligt)
    :mEnableValidationLayers(enableValidationLayers)
    ,mValidation(validation)
    , mMaxFramesInFligt(maxFramesInFligt)
{

}

Device::~Device()
{
    if (mSyncObjectManager)
    {
        delete mSyncObjectManager;
    }

    if (mCommand)
    {
        delete mCommand;
    }

    if (mFrameBuffer)
    {
        delete mFrameBuffer;
    }

    if (mManagers)
    {
        delete mManagers;
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

    if (mScene)
    {
        delete mScene;
    }

    vkDestroyCommandPool(mLogicalDevice, mCopyCommandPool, nullptr);

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
    createManagers();
    createFrameBuffer();
    createCommand();
    createSyncObjectManager();
    createScene();
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

void Device::drawFrame()
{
    VkFence fence = mSyncObjectManager->geFrameFence(mFrameIndex);
    VkCommandBuffer commandBuffer = mCommand->get(mFrameIndex);

    vkWaitForFences(mLogicalDevice, 1, &fence, VK_TRUE, UINT64_MAX);
    vkResetFences(mLogicalDevice, 1, &fence);

    uint32_t imageIndex;

    VkSemaphore backBufferAvailableSemaphore = mSyncObjectManager->getBackBufferReadySemaphore(mFrameIndex);
    vkAcquireNextImageKHR(mLogicalDevice, mSwapChain->get(), UINT64_MAX, backBufferAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

    // update WVP matrix constant buffer
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    ConstantBufferManager* constantBufferManager = mManagers->getConstantBufferManager();
    constantBufferManager->updateWVPConstantBuffer(mFrameIndex, time, mSwapChain->getExtent());

    vkResetCommandBuffer(commandBuffer, 0);
    mCommand->recordCommandBuffer(commandBuffer, mScene, mSwapChain, mFrameBuffer, mManagers, imageIndex);
    
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { backBufferAvailableSemaphore };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    VkSemaphore signalSemaphores[] = { mSyncObjectManager->getPresentReadySemaphore(mFrameIndex) };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(mGraphicsQueue->get(), 1, &submitInfo, fence) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { mSwapChain->get() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    vkQueuePresentKHR(mPresentQueue->get(), &presentInfo);

    mFrameIndex = (mFrameIndex + 1) % mMaxFramesInFligt;
}

void Device::waitIdle()
{
    vkDeviceWaitIdle(mLogicalDevice);
}

void Device::createBuffer(VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkBuffer& buffer,
    VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferCreateInfo{};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size = size;
    bufferCreateInfo.usage = usage;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(mLogicalDevice, 
        &bufferCreateInfo, 
        nullptr, 
        &buffer) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(mLogicalDevice, buffer, &memoryRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memoryRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(mLogicalDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(mLogicalDevice, buffer, bufferMemory, 0);
}

void Device::copyBuffer(VkBuffer srcBuffer,
    VkBuffer dstBuffer,
    VkDeviceSize size)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(mCopyCommandBuffer, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(mCopyCommandBuffer, 
        srcBuffer, 
        dstBuffer, 
        1, 
        &copyRegion);

    vkEndCommandBuffer(mCopyCommandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &mCopyCommandBuffer;

    vkQueueSubmit(mGraphicsQueue->get(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(mGraphicsQueue->get());
}

uint32_t Device::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(mPhysicalDevice, &memoryProperties);

    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
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

void Device::createManagers()
{
    mManagers = new Managers(this, mSwapChain);
}

void Device::createFrameBuffer()
{
    mFrameBuffer = new FrameBuffer(mLogicalDevice, mSwapChain, mManagers);
}

void Device::createCommand()
{
    mCommand = new Command(mPhysicalDevice, mLogicalDevice, mSurface);

    // create copy command
    Queue::QueueFamilyIndices queueFamilyIndices = Queue::findQueueFamilies(mPhysicalDevice, mSurface);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(mLogicalDevice,
        &poolInfo,
        nullptr,
        &mCopyCommandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create copy command pool!");
    }

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = mCopyCommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(mLogicalDevice,
        &allocInfo,
        &mCopyCommandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate copy command buffer!");
    }
}

void Device::createSyncObjectManager()
{
    mSyncObjectManager = new SyncObjectManager(mLogicalDevice);
}

void Device::createScene()
{
    mScene = new SimpleScene(this);
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