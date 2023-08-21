#pragma once

#define GLFW_INCLUDE_VULKAN
#include "Pipeline.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <array>

class Managers;

class PBRPipeline : public Pipeline
{
public:
	
	PBRPipeline(VkDevice logicalDevice, 
		Scene* scene, 
		Managers* managers);
	virtual ~PBRPipeline();

public:
	struct MaterialValue {
		glm::vec4 albedoFactor;

		// metalRoughnessFactor_MapORValue.r is metalRoughnessFactor
		// metalRoughnessFactor_MapORValue.g > 0, albedo value from map; metalRoughnessFactor_MapORValue.g < 0, albedo value from albedoFactor
		// metalRoughnessFactor_MapORValue.b > 0, metalRoughness value from map; metalRoughnessFactor_MapORValue.g < 0, metalRoughness value from metalRoughnessFactor
		// metalRoughnessFactor_MapORValue.a > 0, normal map exist; metalRoughnessFactor_MapORValue.a < 0, no normal map
		glm::vec4 metalRoughnessFactor_MapORValue;
	};
	

public:
	virtual void setupShaderStage(std::vector<VkPipelineShaderStageCreateInfo>& infos);
	virtual void setupVertexInputState(VkPipelineVertexInputStateCreateInfo& info);
	virtual void setupPipelineLayout(VkPipelineLayoutCreateInfo& info);
	virtual VkRenderPass getRenderPass();

protected:
	std::vector<MaterialValue> mMaterialPushConstants;
};