#include "TransitDepthMap.h"
#include "Device.h"

TransitDepthMap::TransitDepthMap(Device* device,
    unsigned int maxFramesInFligt)
    :TransitResource(device, maxFramesInFligt)
{
    // hard-coded shadow map resolution
    mWidth = 2048;
    mHeight = 2048;

    create();
}

TransitDepthMap::~TransitDepthMap()
{
 
}

void TransitDepthMap::create()
{
    unsigned int maxFramesInFlight = mDevice->getMaxFramesInFlight();

	VkFormat format = VK_FORMAT_D32_SFLOAT;

	for (unsigned int i = 0; i < mMaxFramesInFligt; ++i)
	{

		mDevice->create2DImage(
			mWidth,
			mHeight,
			format,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			0,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			mImages[i],
			mDeviceMemories[i]
		);

		// create image view
		mImageViews[i] = mDevice->createImageView(mImages[i],
			format,
			VK_IMAGE_ASPECT_DEPTH_BIT);

		mSamplers[i] = mDevice->createShadowMapSampler();
	}

}