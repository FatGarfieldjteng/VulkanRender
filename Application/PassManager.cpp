#include "PassManager.h"
#include "SwapChain.h"
#include "depthStencilBuffer.h"

#include <stdexcept>

PassManager::PassManager(VkDevice logicalDevice, 
    SwapChain* swapChain,
    DepthStencilBuffer* depthStencilBuffer)
	:mLogicalDevice(logicalDevice),
    mSwapChain(swapChain),
    mDepthStencilBuffer(depthStencilBuffer)
{
	createPasses();
}

PassManager::~PassManager()
{
	std::map<std::string, VkRenderPass>::iterator it;

	for (it = mIDToPass.begin(); it != mIDToPass.end(); ++it)
	{
        vkDestroyRenderPass(mLogicalDevice, it->second, nullptr);
	}
}

void PassManager::createPasses()
{
	createSimplePass();
}

void PassManager::createSimplePass()
{
    VkRenderPass pass = VK_NULL_HANDLE;

    // color attachment, aka. back buffer
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = mSwapChain->getFormat();
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // depth stencil attachment, aka. depth stencil buffer
    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = mDepthStencilBuffer->getFormat();
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpassDesc{};
    subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDesc.colorAttachmentCount = 1;
    subpassDesc.pColorAttachments = &colorAttachmentRef;
    subpassDesc.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    VkAttachmentDescription attachments[2] = { colorAttachment, depthAttachment };

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 2;
    renderPassInfo.pAttachments = &attachments[0];
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpassDesc;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(mLogicalDevice, 
        &renderPassInfo, 
        nullptr, 
        &pass) != VK_SUCCESS) 
    {
        throw std::runtime_error("failed to create render pass!");
    }

	addPass("SimplePass", pass);
}

void PassManager::addPass(const std::string& ID, VkRenderPass pass)
{
	mIDToPass[ID] = pass;
}

VkRenderPass PassManager::getPass(const std::string& ID)
{
	std::map<std::string, VkRenderPass >::iterator it = mIDToPass.find(ID);

	if (it != mIDToPass.end())
	{
		return  it->second;
	}
	else
	{
		return VK_NULL_HANDLE;
	}
}