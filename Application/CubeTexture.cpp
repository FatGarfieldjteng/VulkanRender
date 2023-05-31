
#include "CubeTexture.h"
#include "Device.h"
#include "Bitmap.h"
#include "stb/stb_image.h"
#include "stb/stb_image_resize.h"
#include <stdexcept>
#include <iostream>
#include <filesystem>

CubeTexture::CubeTexture(Device* device)
    :mDevice(device)
{
    
}

CubeTexture::~CubeTexture()
{
    vkDestroySampler(mDevice->getLogicalDevice(), 
        mSampler, nullptr);
    vkDestroyImageView(mDevice->getLogicalDevice(), 
        mImageView, 
        nullptr);

    vkDestroyImage(mDevice->getLogicalDevice(), 
        mImage, 
        nullptr);

    vkFreeMemory(mDevice->getLogicalDevice(), 
        mDeviceMemory, 
        nullptr);
}

void CubeTexture::load(const std::string& file)
{
    // load image
    const float* srcpixels = stbi_loadf(file.c_str(),
        &mOriginalWidth,
        &mOriginalHeight,
        &mChannels, 
        3);

    std::vector<float> pixel32bit(mOriginalWidth * mOriginalHeight * 4);

    float* dstPixels = pixel32bit.data();

    // Convert 24-bit image to 32-bit image
    const int numPixels = mOriginalWidth * mOriginalHeight;
    for (int i = 0; i != numPixels; i++)
    {
        *dstPixels++ = *srcpixels++;
        *dstPixels++ = *srcpixels++;
        *dstPixels++ = *srcpixels++;
        *dstPixels++ = 1.0f;
    }

    stbi_image_free((void*)srcpixels);

    Bitmap in(mOriginalWidth, mOriginalHeight, 4, eBitmapFormat_Float, pixel32bit.data());
   
    Bitmap out = convertEquirectangularMapToVerticalCross(in);

    Bitmap cube = convertVerticalCrossToCubeMapFaces(out);

    mDevice->createImage(vkDev.device, vkDev.physicalDevice, texWidth, texHeight, texFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory, flags);
















    stbi_uc* pixels = stbi_load(file.c_str(),
        &mWidth, 
        &mHeight, 
        &mChannels, 
        STBI_rgb_alpha);

    VkDeviceSize imageSize = static_cast<VkDeviceSize>(mWidth * mHeight * 4);

    if (!pixels) {
        throw std::runtime_error("failed to load texture image!");
    }

    // create staging buffer and copy pixels into it
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    mDevice->createBuffer(imageSize, 
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
        stagingBuffer, 
        stagingBufferMemory);

    void* data;
    vkMapMemory(mDevice->getLogicalDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
    std::memcpy(data, pixels, static_cast<size_t>(imageSize));
    vkUnmapMemory(mDevice->getLogicalDevice(), stagingBufferMemory);
    stbi_image_free(pixels);

    mDevice->createImage(mWidth, 
        mHeight, 
        VK_FORMAT_R8G8B8A8_SRGB, 
        VK_IMAGE_TILING_OPTIMAL, 
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
        mImage,
        mDeviceMemory);

    mDevice->transitImageLayout(mImage,
        VK_FORMAT_R8G8B8A8_SRGB, 
        VK_IMAGE_LAYOUT_UNDEFINED, 
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    mDevice->copyBufferToImage(stagingBuffer, 
        mImage, 
        static_cast<uint32_t>(mWidth), 
        static_cast<uint32_t>(mHeight));

    mDevice->transitImageLayout(mImage,
        VK_FORMAT_R8G8B8A8_SRGB, 
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

    vkDestroyBuffer(mDevice->getLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(mDevice->getLogicalDevice(), stagingBufferMemory, nullptr);

    // create image view
    mImageView = mDevice->createImageView(mImage, 
        VK_FORMAT_R8G8B8A8_SRGB, 
        VK_IMAGE_ASPECT_COLOR_BIT);

    // crete sampler
    mSampler = mDevice->createSampler();
}

