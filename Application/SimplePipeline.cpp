#include "SimplePipeline.h"
#include "Managers.h"
#include "Shadermanager.h"
#include "PassManager.h"
#include "FormatManager.h"
#include "ConstantBufferManager.h"
#include "ConstantBuffer.h"
#include "PCTVertexFormat.h"

#include <vector>
#include <stdexcept>

SimplePipeline::SimplePipeline(VkDevice logicalDevice, Managers* managers)
    :Pipeline(logicalDevice, managers)
{

}

SimplePipeline::~SimplePipeline()
{
}

void SimplePipeline::setupShaderStage(std::vector<VkPipelineShaderStageCreateInfo>& infos)
{
    ShaderManager* shaderManager = mManagers->getShaderManager();

    infos.resize(2);
    infos[0] = shaderManager->getVS("SimpleVS");
    infos[1] = shaderManager->getPS("SimplePS");
}

void SimplePipeline::setupVertexInputState(VkPipelineVertexInputStateCreateInfo& info)
{
    FormatManager* formatManager = mManagers->getFormatManager();
    PCTVertexFormat* format = (PCTVertexFormat*)formatManager->getFormat("PCTFormat");

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    info.vertexBindingDescriptionCount = 1;
    info.pVertexBindingDescriptions = &(format->mBinding);
    info.vertexAttributeDescriptionCount = (uint32_t)format->mAttribute.size();
    info.pVertexAttributeDescriptions = format->mAttribute.data();
}

void SimplePipeline::setupPipelineLayout(VkPipelineLayoutCreateInfo& info)
{
    ConstantBufferManager* constantBufferManager = mManagers->getConstantBufferManager();

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    info.setLayoutCount = 1;
    info.pSetLayouts = constantBufferManager->getConstantBuffer("Simple")->getDescriptorSetLayout();
    info.pushConstantRangeCount = 0;

    if (vkCreatePipelineLayout(mLogicalDevice,
        &info,
        nullptr,
        &mPipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

VkRenderPass SimplePipeline::getRenderPass()
{
    PassManager* passManager = mManagers->getPassManager();
    return passManager->getPass("SimplePass");
}