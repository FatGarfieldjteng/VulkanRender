#include "ShaderManager.h"
#include <fstream>

ShaderManager::ShaderManager(VkDevice logicalDevice)
	:mLogicalDevice(logicalDevice)
{
	createShaders();
}

ShaderManager::~ShaderManager()
{
	std::map<std::string, VkShaderModule>::iterator itVS;

	for (itVS = mIDToVS.begin(); itVS != mIDToVS.end(); itVS++)
	{
		vkDestroyShaderModule(mLogicalDevice, itVS->second, nullptr);
	}

	std::map<std::string, VkShaderModule>::iterator itPS;

	for (itPS = mIDToPS.begin(); itPS != mIDToPS.end(); itPS++)
	{
		vkDestroyShaderModule(mLogicalDevice, itPS->second, nullptr);
	}

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

	addVS("SimpleVS", shaderModule);
}

void ShaderManager::createSimplePS()
{
	std::string path = shaderPath();

	std::string shaderPath = path + "SimplePS.frag.spv";
	std::vector<char> bytecode = readFile(shaderPath);
	VkShaderModule shaderModule = createShaderModule(bytecode);

	addPS("SimplePS", shaderModule);
}

void ShaderManager::addVS(const std::string& ID, VkShaderModule vs)
{
	mIDToVS[ID] = vs;
}

void ShaderManager::addPS(const std::string& ID, VkShaderModule ps)
{
	mIDToPS[ID] = ps;
}

VkShaderModule ShaderManager::getVS(const std::string& ID)
{
	std::map<std::string, VkShaderModule >::iterator it = mIDToVS.find(ID);

	if (it != mIDToVS.end())
	{
		return  it->second;
	}
	else
	{
		return VK_NULL_HANDLE;
	}
}

VkShaderModule ShaderManager::getPS(const std::string& ID)
{
	std::map<std::string, VkShaderModule >::iterator it = mIDToPS.find(ID);

	if (it != mIDToPS.end())
	{
		return  it->second;
	}
	else
	{
		return VK_NULL_HANDLE;
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