#include "DepthStencilBuffer.h"
#include "Device.h"
#include <stdexcept>

DepthStencilBuffer::DepthStencilBuffer(Device* device)
    :mDevice(device)
{
    
}

DepthStencilBuffer::~DepthStencilBuffer()
{
    vkDestroyImageView(mDevice->getLogicalDevice(), mImageView, nullptr);
    vkDestroyImage(mDevice->getLogicalDevice(), mImage, nullptr);
    vkFreeMemory(mDevice->getLogicalDevice(), mDeviceMemory, nullptr);
}

void DepthStencilBuffer::init(VkExtent2D extent)
{
    mFormat = mDevice->findDepthFormat();
    mDevice->createImage(extent.width, extent.height,
        mFormat,
        VK_IMAGE_TILING_OPTIMAL,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        mImage,
        mDeviceMemory);

    mImageView = mDevice->createImageView(mImage, 
        mFormat, 
        VK_IMAGE_ASPECT_DEPTH_BIT);
}

VkFormat DepthStencilBuffer::getFormat()
{
    return mFormat;
}

VkImageView DepthStencilBuffer::getView()
{
    return mImageView;
}