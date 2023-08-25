#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <map>
#include <string>
#include <filesystem>

class ShaderManager
{
public:

    static std::string shaderPath()
    {
        std::filesystem::path path = std::filesystem::current_path();
        return (path.parent_path().parent_path().string() + "\\Application\\Shader\\");
    }

    ShaderManager(VkDevice logicalDevice);

    ~ShaderManager();

public:
    void createShaders();

    void addVS(const std::string& ID, VkPipelineShaderStageCreateInfo vs);
    void addPS(const std::string& ID, VkPipelineShaderStageCreateInfo ps);

    VkPipelineShaderStageCreateInfo getVS(const std::string& ID);
    VkPipelineShaderStageCreateInfo getPS(const std::string& ID);

private:
    void createSimpleVS();
    void createSimplePS();

    void createPBRVS();
    void createPBRPS();

    // no shadowPS needed
    void createShadowVS();

    // helper functions
    std::vector<char> ShaderManager::readFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code);

private:
    
    VkDevice mLogicalDevice = VK_NULL_HANDLE;
    std::map<std::string, VkPipelineShaderStageCreateInfo > mIDToVS;
    std::map<std::string, VkPipelineShaderStageCreateInfo > mIDToPS;

    std::vector< VkShaderModule> mScratchPad;
};
