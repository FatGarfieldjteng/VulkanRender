#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <map>

class Pipeline;
class Managers;

class PipelineManager
{
public:

    PipelineManager(VkDevice logicalDevice, Managers* managers);

    ~PipelineManager();

public:
    void createPipelines();

    void addPipeline(const std::string& ID, Pipeline* pipeline);
    
    Pipeline* getPipeline(const std::string& ID);
    
private:
    void createSimplePipeline();

    void createPBRPipeline();

private:
    
    VkDevice mLogicalDevice = VK_NULL_HANDLE;
    Managers* mManagers = nullptr;
    std::map<std::string, Pipeline*> mIDToPipeline;
};
