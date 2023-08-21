#include "Pipeline.h"

#include <vector>
#include <stdexcept>

Pipeline::Pipeline(VkDevice logicalDevice, 
    Scene* scene,
    Managers* managers)
    :mLogicalDevice(logicalDevice),
    mScene(scene),
    mManagers(managers)
{

}

Pipeline::~Pipeline()
{
    vkDestroyPipeline(mLogicalDevice, mPipeline, nullptr);
    vkDestroyPipelineLayout(mLogicalDevice, mPipelineLayout, nullptr);
}

void Pipeline::create()
{
    // shaders
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    setupShaderStage(shaderStages);

    // vertex input layout
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    setupVertexInputState(vertexInputInfo);
    
    // IA
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    setupInputAssemblyState(inputAssembly);

    // view port and scissor rect
    VkPipelineViewportStateCreateInfo viewportState{};
    setupViewportState(viewportState);

    // rasterization
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    setupRasterizationState(rasterizer);

    // multisample
    VkPipelineMultisampleStateCreateInfo multisampling{};
    setupMultisampleState(multisampling);

    VkPipelineDepthStencilStateCreateInfo depthStencilState{};
    setupDepthStencilState(depthStencilState);

    // blend
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
        | VK_COLOR_COMPONENT_G_BIT
        | VK_COLOR_COMPONENT_B_BIT
        | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    setupColorBlendState(colorBlending, colorBlendAttachment);

    // dynamic state, which can be set at rendering dynamically without recreate Pipeline
    VkPipelineDynamicStateCreateInfo dynamicState{};
    std::vector<VkDynamicState> dynamicStates =
    {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    setupDynamicState(dynamicState, dynamicStates);

    // root signature
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    setupPipelineLayout(pipelineLayoutInfo);

    VkGraphicsPipelineCreateInfo pipelineInfo{};

    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = (uint32_t)shaderStages.size();
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencilState;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = mPipelineLayout;
    pipelineInfo.renderPass = getRenderPass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(mLogicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mPipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }
}

VkPipeline Pipeline::getPipeline()
{
    return mPipeline;
}

VkPipelineLayout Pipeline::getPipelineLayout()
{
    return mPipelineLayout;
}

void Pipeline::setupInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo& info)
{
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    info.primitiveRestartEnable = VK_FALSE;
}

void Pipeline::setupViewportState(VkPipelineViewportStateCreateInfo& info)
{
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    info.viewportCount = 1;
    info.scissorCount = 1;
}

void Pipeline::setupRasterizationState(VkPipelineRasterizationStateCreateInfo& info)
{
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    info.depthClampEnable = VK_FALSE;
    info.rasterizerDiscardEnable = VK_FALSE;
    info.polygonMode = VK_POLYGON_MODE_FILL;
    info.lineWidth = 1.0f;
    info.cullMode = VK_CULL_MODE_NONE;
    info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    info.depthBiasEnable = VK_FALSE;
}

void Pipeline::setupMultisampleState(VkPipelineMultisampleStateCreateInfo& info)
{
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    info.sampleShadingEnable = VK_FALSE;
    info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
}

void Pipeline::setupDepthStencilState(VkPipelineDepthStencilStateCreateInfo& info)
{
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    info.depthTestEnable = VK_TRUE;
    info.depthWriteEnable = VK_TRUE;
    info.depthCompareOp = VK_COMPARE_OP_LESS;
    info.depthBoundsTestEnable = VK_FALSE;
    info.stencilTestEnable = VK_FALSE;
}

void Pipeline::setupColorBlendState(VkPipelineColorBlendStateCreateInfo& info, 
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

void Pipeline::setupDynamicState(VkPipelineDynamicStateCreateInfo& info,
    const std::vector<VkDynamicState>& dynamicStates)
{
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    info.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    info.pDynamicStates = dynamicStates.data();
}