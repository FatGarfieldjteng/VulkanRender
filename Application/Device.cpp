#include "Device.h"
#include "Validation.h"
#include "SwapChain.h"
#include "DepthStencilBuffer.h"
#include "ShaderManager.h"
#include "Pipeline.h"
#include "PassManager.h"
#include "Managers.h"
#include "FrameBuffer.h"
#include "Command.h"
#include "SyncObjectManager.h"
#include "ConstantBufferManager.h"
#include "SimpleScene.h"
#include "Camera.h"
#include "BoundingBox.h"

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

    if (mDepthStencilBuffer)
    {
        delete mDepthStencilBuffer;
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

    if (mCamera)
    {
        delete mCamera;
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
    createDepthStencilBuffer();
    createCommand();

    createScene();
    createCamera();

    createManagers();
    
    
    createFrameBuffer();
    createSyncObjectManager();
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
    mCommand->recordCommandBuffer(commandBuffer, 
        mScene, 
        mSwapChain, 
        mFrameBuffer, 
        mManagers, 
        imageIndex, 
        mFrameIndex);
    
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

VkFormat Device::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(mPhysicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format!");
}

VkFormat Device::findDepthFormat()
{
    return findSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
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

void Device::beginCopyCommand()
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(mCopyCommandBuffer, &beginInfo);
}

void Device::endCopyCommand()
{
    vkEndCommandBuffer(mCopyCommandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &mCopyCommandBuffer;

    vkQueueSubmit(mGraphicsQueue->get(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(mGraphicsQueue->get());
}

void Device::copyBuffer(VkBuffer srcBuffer,
    VkBuffer dstBuffer,
    VkDeviceSize size)
{
    beginCopyCommand();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(mCopyCommandBuffer, 
        srcBuffer, 
        dstBuffer, 
        1, 
        &copyRegion);

    endCopyCommand();
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
    deviceFeatures.samplerAnisotropy = VK_TRUE;

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

void Device::createDepthStencilBuffer()
{
    mDepthStencilBuffer = new DepthStencilBuffer(this);
    mDepthStencilBuffer->init(mSwapChain->getExtent());
}

void Device::createManagers()
{
    mManagers = new Managers(this, mSwapChain, mDepthStencilBuffer, mScene);
}

void Device::createFrameBuffer()
{
    mFrameBuffer = new FrameBuffer(mLogicalDevice, mSwapChain, mDepthStencilBuffer, mManagers);
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

void Device::createCamera()
{
    mCamera = new Camera();

        
    glm::vec3 center = mScene->getBBox()->center();
    glm::vec3 extent = mScene->getBBox()->extent();
    glm::vec3 eye = center - glm::vec3(0.0f, 2.0f * extent.y, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    
    mCamera->setupLookAt(eye, center, up);

    VkExtent2D windowSize = mSwapChain->getExtent();

    mCamera->setupPespective(90.0f, windowSize.width / (float)windowSize.height, 0.1f, 100000.0f);

    mCamera->computeViewProj();
}

bool Device::isDeviceSuitable(VkPhysicalDevice physicalDevice)
{
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

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
        && swapChainAdequate
        && deviceFeatures.samplerAnisotropy);
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

void Device::createImage(uint32_t width,
    uint32_t height, 
    VkFormat format, 
    VkImageTiling tiling, 
    VkImageUsageFlags usage, 
    VkMemoryPropertyFlags properties, 
    VkImage& image, 
    VkDeviceMemory& imageMemory) 
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(mLogicalDevice, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(mLogicalDevice, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(mLogicalDevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(mLogicalDevice, image, imageMemory, 0);
}

VkImageView Device::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags) 
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(mLogicalDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}

VkSampler Device::createSampler()
{
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(mPhysicalDevice, &properties);

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    VkSampler sampler;

    if (vkCreateSampler(mLogicalDevice,
        &samplerInfo,
        nullptr,
        &sampler) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create texture sampler!");
    }

    return sampler;
}

void Device::transitImageLayout(VkImage image, 
    VkFormat format, 
    VkImageLayout oldLayout, 
    VkImageLayout newLayout) 
{
    beginCopyCommand();

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(
        mCopyCommandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );

    endCopyCommand();
}

void Device::copyBufferToImage(VkBuffer buffer,
    VkImage image,
    uint32_t width,
    uint32_t height)
{
    beginCopyCommand();
    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = {
        width,
        height,
        1
    };

    vkCmdCopyBufferToImage(mCopyCommandBuffer, buffer, 
        image, 
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
        1, &region);

    endCopyCommand();
}