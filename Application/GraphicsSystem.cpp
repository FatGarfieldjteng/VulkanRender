#include "GraphicsSystem.h"

GraphicsSystem::GraphicsSystem()
{

}

GraphicsSystem::~GraphicsSystem()
{

}

void GraphicsSystem::run() 
{
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
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

}

void GraphicsSystem::mainLoop() 
{
    while (!glfwWindowShouldClose(mWindow)) {
        glfwPollEvents();
    }
}

void GraphicsSystem::cleanup() 
{
    glfwDestroyWindow(mWindow);

    glfwTerminate();
}