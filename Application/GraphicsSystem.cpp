#include "GraphicsSystem.h"
#include "Validation.h"
#include "Device.h"
#include "Queue.h"

#include <stdexcept>

GraphicsSystem::GraphicsSystem()
{

}

GraphicsSystem::~GraphicsSystem()
{
    if (mEnableValidationLayers)
    {
        delete mValidation;
    }
}

void GraphicsSystem::run() 
{
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void GraphicsSystem::createInstance()
{
    if (mEnableValidationLayers)
    {
        createValidation();
        
    }

    if (!mValidation->checkValidationLayerSupport()) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "First Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = getGLFWRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();
    
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (mEnableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(mValidation->getValidationLayersSize());
        createInfo.ppEnabledLayerNames = mValidation->getValidationLayersData();
        mValidation->setupVkDebugUtilsMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateInstance(&createInfo, nullptr, &mVkInstance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create Vulkan instance!");
    }

}

void GraphicsSystem::createDebugMessenger()
{
    mValidation->createDebugMessenger(mVkInstance);
}

void GraphicsSystem::createValidation()
{
    mValidation = new Validation();
}

void GraphicsSystem::createDevice()
{
    mDevice = new Device();
    mDevice->create(mVkInstance);
}

void GraphicsSystem::createQueue()
{
    mQueue = new Queue();
    mQueue->acquireQueue(mDevice);
}

void GraphicsSystem::initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    mWindow = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

void GraphicsSystem::initVulkan() 
{
    createInstance();
    createDebugMessenger();
    createDevice();
    createQueue();
}

void GraphicsSystem::mainLoop() 
{
    while (!glfwWindowShouldClose(mWindow)) {
        glfwPollEvents();
    }
}

void GraphicsSystem::cleanup() 
{
    if (mEnableValidationLayers) 
    {
       mValidation->destroyDebugMessenger(mVkInstance);
    }

    vkDestroyInstance(mVkInstance, nullptr);

    glfwDestroyWindow(mWindow);

    glfwTerminate();
}


std::vector<const char*> GraphicsSystem::getGLFWRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (mEnableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}