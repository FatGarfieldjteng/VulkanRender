#include "ShadowPipeline.h"
#include "Managers.h"
#include "Shadermanager.h"
#include "PassManager.h"
#include "FormatManager.h"
#include "ConstantBufferManager.h"
#include "ConstantBuffer.h"
#include "PBRVertexFormat.h"
#include "SimpleScene.h"
#include "PBRMaterial.h"
#include <vector>
#include <stdexcept>

ShadowPipeline::ShadowPipeline(VkDevice logicalDevice,
    Scene* scene, 
    Managers* managers)
    :Pipeline(logicalDevice, scene, managers)
{

}

ShadowPipeline::~ShadowPipeline()
{
}

void ShadowPipeline::setupShaderStage(std::vector<VkPipelineShaderStageCreateInfo>& infos)
{
    ShaderManager* shaderManager = mManagers->getShaderManager();

    infos.resize(1);
    infos[0] = shaderManager->getVS("ShadowVS");
}

void ShadowPipeline::setupVertexInputState(VkPipelineVertexInputStateCreateInfo& info)
{
    FormatManager* formatManager = mManagers->getFormatManager();
    PBRVertexFormat* format = (PBRVertexFormat*)formatManager->getFormat("PBRFormat");

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    info.vertexBindingDescriptionCount = 1;
    info.pVertexBindingDescriptions = &(format->mBinding);
    info.vertexAttributeDescriptionCount = (uint32_t)format->mAttribute.size();
    info.pVertexAttributeDescriptions = format->mAttribute.data();
}

void ShadowPipeline::setupColorBlendState(VkPipelineColorBlendStateCreateInfo& info,
    const VkPipelineColorBlendAttachmentState& colorBlendAttachmentState)
{
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    info.logicOpEnable = VK_FALSE;
    info.logicOp = VK_LOGIC_OP_COPY;
    info.attachmentCount = 1;
    info.pAttachments = &colorBlendAttachmentState;
    info.blendConstants[0] = 0.0f;
    info.blendConstants[1] = 0.0f;
    info.blendConstants[2] = 0.0f;
    info.blendConstants[3] = 0.0f;
}

void ShadowPipeline::setupDynamicState(VkPipelineDynamicStateCreateInfo& info,
    std::vector<VkDynamicState>& dynamicStates)
{
    dynamicStates.push_back(VK_DYNAMIC_STATE_DEPTH_BIAS);
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    info.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    info.pDynamicStates = dynamicStates.data();
}

void ShadowPipeline::setupPipelineLayout(VkPipelineLayoutCreateInfo& info)
{
    ConstantBufferManager* constantBufferManager = mManagers->getConstantBufferManager();

    info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    info.setLayoutCount = 1;
    info.pSetLayouts = constantBufferManager->getConstantBuffer("Shadow")->getDescriptorSetLayout();
    info.pushConstantRangeCount = 0;

    if (vkCreatePipelineLayout(mLogicalDevice,
        &info,
        nullptr,
        &mPipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

VkRenderPass ShadowPipeline::getRenderPass()
{
    PassManager* passManager = mManagers->getPassManager();
    return passManager->getPass("ShadowPass");
}