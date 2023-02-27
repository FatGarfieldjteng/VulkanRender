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

    void addVS(const std::string& ID, VkShaderModule vs);
    void addPS(const std::string& ID, VkShaderModule ps);

    VkShaderModule getVS(const std::string& ID);
    VkShaderModule getPS(const std::string& ID);

private:
    void createSimpleVS();
    void createSimplePS();

    // helper functions
    std::vector<char> ShaderManager::readFile(const std::string& filename);
    VkShaderModule createShaderModule(const std::vector<char>& code);

private:
    
    VkDevice mLogicalDevice;
    std::map<std::string, VkShaderModule > mIDToVS;
    std::map<std::string, VkShaderModule > mIDToPS;

};
