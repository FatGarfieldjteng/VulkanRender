
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
    float* originalPixel = stbi_loadf(file.c_str(),
        &mOriginalWidth,
        &mOriginalHeight,
        &mChannels,
        3);

    std::vector<float> pixel32bit(mOriginalWidth * mOriginalHeight * 4);

    float* dstPixel = pixel32bit.data();
    float* srcPixel = originalPixel;

    // Convert 24-bit image to 32-bit image
    const int numPixels = mOriginalWidth * mOriginalHeight;
    for (int i = 0; i != numPixels; i++)
    {
        *dstPixel++ = *srcPixel++;
        *dstPixel++ = *srcPixel++;
        *dstPixel++ = *srcPixel++;
        *dstPixel++ = 1.0f;
    }

    stbi_image_free((void*)originalPixel);

    Bitmap equirectangularImage(mOriginalWidth, mOriginalHeight, 4, eBitmapFormat_Float, pixel32bit.data());

    Bitmap verticalCrossImage = convertEquirectangularMapToVerticalCross(equirectangularImage);

    Bitmap cubeImage = convertVerticalCrossToCubeMapFaces(verticalCrossImage);

    mDevice->create2DImage(cubeImage.w_, cubeImage.h_,
        VK_FORMAT_R32G32B32A32_SFLOAT, 
        VK_IMAGE_TILING_OPTIMAL, 
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
        mImage, 
        mDeviceMemory);

    uint32_t layerCount = 6;

    VkDeviceSize imageSize = static_cast<VkDeviceSize>(cubeImage.w_ * cubeImage.h_ * layerCount * 16);

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
    std::memcpy(data, cubeImage.data_.data(), static_cast<size_t>(imageSize));
    vkUnmapMemory(mDevice->getLogicalDevice(), stagingBufferMemory);
    
    mDevice->transitImageLayout(mImage,
        VK_FORMAT_R32G32B32A32_SFLOAT,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        layerCount);

    mDevice->copyBufferToImage(stagingBuffer,
        mImage,
        static_cast<uint32_t>(cubeImage.w_),
        static_cast<uint32_t>(cubeImage.h_), 
        layerCount);

    mDevice->transitImageLayout(mImage,
        VK_FORMAT_R32G32B32A32_SFLOAT,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        layerCount);

    vkDestroyBuffer(mDevice->getLogicalDevice(), stagingBuffer, nullptr);
    vkFreeMemory(mDevice->getLogicalDevice(), stagingBufferMemory, nullptr);


    // create image view
    mImageView = mDevice->createImageView(mImage,
        VK_FORMAT_R32G32B32A32_SFLOAT,
        VK_IMAGE_ASPECT_COLOR_BIT,
        VK_IMAGE_VIEW_TYPE_CUBE,
        layerCount);

    // crete sampler
    mSampler = mDevice->createSampler();
}

