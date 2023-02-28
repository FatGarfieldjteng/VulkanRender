#include "PassManager.h"
#include "SwapChain.h"
#include <stdexcept>

PassManager::PassManager(VkDevice logicalDevice, SwapChain* swapChain)
	:mLogicalDevice(logicalDevice),
    mSwapChain(swapChain)
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

    VkAttachmentDescription attachmentDesc{};
    attachmentDesc.format = mSwapChain->getFormat();
    attachmentDesc.samples = VK_SAMPLE_COUNT_1_BIT;
    attachmentDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachmentDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachmentDesc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachmentDesc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachmentDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachmentDesc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference attachmentRef{};
    attachmentRef.attachment = 0;
    attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpassDesc{};
    subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDesc.colorAttachmentCount = 1;
    subpassDesc.pColorAttachments = &attachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &attachmentDesc;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpassDesc;

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