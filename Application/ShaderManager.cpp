#include "ShaderManager.h"
#include <fstream>

ShaderManager::ShaderManager(VkDevice logicalDevice)
	:mLogicalDevice(logicalDevice)
{
	createShaders();
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::createShaders()
{
	createSimpleVS();
	createSimplePS();

}

void ShaderManager::createSimpleVS()
{
	std::string path = shaderPath();

	std::string shaderPath = path + "SimpleVS.vert.spv";
	std::vector<char> bytecode = readFile(shaderPath);

	VkShaderModule shaderModule = createShaderModule(bytecode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = shaderModule;
	vertShaderStageInfo.pName = "main";

	vkDestroyShaderModule(mLogicalDevice, shaderModule, nullptr);

	addVS("SimpleVS", vertShaderStageInfo);
}

void ShaderManager::createSimplePS()
{
	std::string path = shaderPath();

	std::string shaderPath = path + "SimplePS.frag.spv";
	std::vector<char> bytecode = readFile(shaderPath);
	VkShaderModule shaderModule = createShaderModule(bytecode);

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = shaderModule;
	fragShaderStageInfo.pName = "main";

	addPS("SimplePS", fragShaderStageInfo);

	vkDestroyShaderModule(mLogicalDevice, shaderModule, nullptr);
}

void ShaderManager::addVS(const std::string& ID, VkPipelineShaderStageCreateInfo vs)
{
	mIDToVS[ID] = vs;
}

void ShaderManager::addPS(const std::string& ID, VkPipelineShaderStageCreateInfo ps)
{
	mIDToPS[ID] = ps;
}

VkPipelineShaderStageCreateInfo ShaderManager::getVS(const std::string& ID)
{
	std::map<std::string, VkPipelineShaderStageCreateInfo >::iterator it = mIDToVS.find(ID);

	if (it != mIDToVS.end())
	{
		return  it->second;
	}
	else
	{
		VkPipelineShaderStageCreateInfo info = {};
		return info;
	}
}

VkPipelineShaderStageCreateInfo ShaderManager::getPS(const std::string& ID)
{
	std::map<std::string, VkPipelineShaderStageCreateInfo >::iterator it = mIDToPS.find(ID);

	if (it != mIDToPS.end())
	{
		return  it->second;
	}
	else
	{
		VkPipelineShaderStageCreateInfo info = {};
		return info;
	}
}

std::vector<char> ShaderManager::readFile(const std::string& filename) 
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

VkShaderModule ShaderManager::createShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(mLogicalDevice, 
		&createInfo, 
		nullptr, 
		&shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}