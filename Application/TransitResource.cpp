#include "TransitResource.h"
#include "Device.h"

TransitResource::TransitResource(Device* device,
    unsigned int maxFramesInFligt)
    :mDevice(device),
    mMaxFramesInFligt(maxFramesInFligt)
{
    mImages.resize(mMaxFramesInFligt);
    mImageViews.resize(mMaxFramesInFligt);
    mSamplers.resize(mMaxFramesInFligt);
    mDeviceMemories.resize(mMaxFramesInFligt);
}

TransitResource::~TransitResource()
{
    for (unsigned int i = 0; i < mMaxFramesInFligt; ++i)
    {
        vkDestroySampler(mDevice->getLogicalDevice(),
            mSamplers[i], nullptr);
        vkDestroyImageView(mDevice->getLogicalDevice(),
            mImageViews[i],
            nullptr);

        vkDestroyImage(mDevice->getLogicalDevice(),
            mImages[i],
            nullptr);

        vkFreeMemory(mDevice->getLogicalDevice(),
            mDeviceMemories[i],
            nullptr);
    }
}