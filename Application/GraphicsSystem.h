#pragma once

#include "GraphicsSystem.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>

class GraphicsSystem
{
public:

	GraphicsSystem();
	~GraphicsSystem();

public:
    void run();

private:

    // create Vulkan instance
    void createInstance();

    void initWindow();

    void initVulkan();

    void mainLoop();

    void cleanup();

private:
    bool checkValidationLayerSupport();

    std::vector<const char*> getGLFWRequiredExtensions();

    void setupDebugMessenger();

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, 
        VkDebugUtilsMessageTypeFlagsEXT messageType, 
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
        void* pUserData) 
    {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }

private:

    VkInstance mVkInstance = nullptr;

#ifdef NDEBUG
    const bool mEnableValidationLayers = false;
#else
    const bool mEnableValidationLayers = true;
#endif

    const std::vector<const char*> mValidationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    VkDebugUtilsMessengerEXT mDebugMessenger;

    const uint32_t WIDTH = 1024;
    const uint32_t HEIGHT = 768;
    GLFWwindow* mWindow = nullptr;
};