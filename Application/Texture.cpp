
#include "Texture.h"
#include "Device.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <stdexcept>
#include <iostream>
#include <filesystem>

Texture::Texture(Device* device)
    :mDevice(device)
{
    
}

Texture::~Texture()
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

void Texture::load(const std::string& file)
{
    // load image
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

    mDevice->create2DImage(mWidth, 
        mHeight, 
        VK_FORMAT_R8G8B8A8_SRGB, 
        VK_IMAGE_TILING_OPTIMAL, 
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
        0,
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

