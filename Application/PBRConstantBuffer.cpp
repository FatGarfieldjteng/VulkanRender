#include "PBRConstantBuffer.h"
#include "Device.h"
#include "Scene.h"
#include "SimpleScene.h"
#include "Texture.h"
#include "PBRMaterial.h"
#include "CubeTexture.h"
#include "GLITexture.h"
#include "VulkanHelper.h"
#include <stdexcept>
#include <array>

PBRConstantBuffer::PBRConstantBuffer(Device* device,
    Scene* scene,
    unsigned int maxFramesInFligt)
    :ConstantBuffer(device, scene, maxFramesInFligt)
{
    init();
}

PBRConstantBuffer::~PBRConstantBuffer()
{

}

void PBRConstantBuffer::update(uint32_t frameIndex, void* data, size_t size)
{

}

void PBRConstantBuffer::createDescriptorPool()
{
    SimpleScene* simpleScene = dynamic_cast<SimpleScene*>(mScene);
    const std::vector< PBRMaterial*>& PBRMaterials = simpleScene->getPBRMaterials();

    int materialCount = (int)PBRMaterials.size();

    std::vector<VkDescriptorPoolSize> poolSizes;

    VkDescriptorPoolSize uniformPoolSize{};
    uniformPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uniformPoolSize.descriptorCount = mMaxFramesInFligt * materialCount;
    poolSizes.push_back(uniformPoolSize);
   
    VkDescriptorPoolSize samplerPoolSize{};
    samplerPoolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerPoolSize.descriptorCount = mMaxFramesInFligt * materialCount * 6;
    poolSizes.push_back(samplerPoolSize);
   

    VkDescriptorPoolCreateInfo poolCreateInfo{};
    poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolCreateInfo.pNext = nullptr;
    poolCreateInfo.flags = 0;
    poolCreateInfo.maxSets = static_cast<uint32_t>(mMaxFramesInFligt * materialCount),
        poolCreateInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolCreateInfo.pPoolSizes = poolSizes.empty() ? nullptr : poolSizes.data();

    vkCreateDescriptorPool(mDevice->getLogicalDevice(), &poolCreateInfo, nullptr, &mDescriptorPool);
}

void PBRConstantBuffer::createDescriptorSetLayout()
{
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    bindings.resize(7);

    bindings[0].binding = 0;
    bindings[0].descriptorCount = 1;
    bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    bindings[0].pImmutableSamplers = nullptr;
    bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

    for (int i = 1; i < 7; i++)
    {
        bindings[i].binding = i;
        bindings[i].descriptorCount = 1;
        bindings[i].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        bindings[i].pImmutableSamplers = nullptr;
        bindings[i].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.pNext = nullptr;
    layoutInfo.flags = 0;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    vkCreateDescriptorSetLayout(mDevice->getLogicalDevice(), &layoutInfo, nullptr, &mDescriptorSetLayout);
}

void PBRConstantBuffer::createDescriptorSets()
{
    SimpleScene* simpleScene = dynamic_cast<SimpleScene*>(mScene);
    const std::vector< PBRMaterial*>& PBRMaterials = simpleScene->getPBRMaterials();
    Texture* defaultTexture = simpleScene->getDefaultTexture();

    int materialCount = (int)PBRMaterials.size();

    std::vector<VkDescriptorSetLayout> layouts(mMaxFramesInFligt * materialCount, mDescriptorSetLayout);

    VkDescriptorSetAllocateInfo allocInfo{};

    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.descriptorPool = mDescriptorPool;
    //allocInfo.descriptorSetCount = frameInFlight * materialCount;
    allocInfo.descriptorSetCount = mMaxFramesInFligt * materialCount;
    allocInfo.pSetLayouts = layouts.data();

    mDescriptorSets.resize(mMaxFramesInFligt * materialCount);

    // allocate descriptor sets
    vkAllocateDescriptorSets(mDevice->getLogicalDevice(), &allocInfo, mDescriptorSets.data());

    for (int frameIndex = 0; frameIndex < mMaxFramesInFligt; ++frameIndex)
    {
        for (int materialIndex = 0; materialIndex < materialCount; ++materialIndex)
        {
            int descriptorIndex = frameIndex * materialCount + materialIndex;
            VkDescriptorSet ds = mDescriptorSets[descriptorIndex];

            // view projection matrix and camera position uniform buffer
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = mUniformBuffers[frameIndex];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(MVPCameraPosConstantBuffer);

            // textures for PBR material
            PBRMaterial* mat = PBRMaterials[materialIndex];

            // albedo map, eg. diffuse
            VkDescriptorImageInfo imageInfoAlbedo{};

            if (mat->mValues.mMetalRoughness_MapORValue.g > 0.0f)
            {
                imageInfoAlbedo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfoAlbedo.imageView = mat->mTextures.mAlbedo->mImageView;
                imageInfoAlbedo.sampler = mat->mTextures.mAlbedo->mSampler;
            }
            else
            {
                // load default image to prevent crash
                imageInfoAlbedo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfoAlbedo.imageView = defaultTexture->mImageView;
                imageInfoAlbedo.sampler = defaultTexture->mSampler;
            }

            VkDescriptorImageInfo imageInfoMeR{};

            if (mat->mValues.mMetalRoughness_MapORValue.b > 0.0f)
            {
                imageInfoMeR.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfoMeR.imageView = mat->mTextures.mMetalRoughness->mImageView;
                imageInfoMeR.sampler = mat->mTextures.mMetalRoughness->mSampler;
            }
            else
            {
                imageInfoMeR.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfoMeR.imageView = defaultTexture->mImageView;
                imageInfoMeR.sampler = defaultTexture->mSampler;
            }

            // normal map
            VkDescriptorImageInfo imageInfoNormal{};

            if (mat->mValues.mMetalRoughness_MapORValue.a > 0.0f)
            {
                imageInfoNormal.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfoNormal.imageView = mat->mTextures.mNormal->mImageView;
                imageInfoNormal.sampler = mat->mTextures.mNormal->mSampler;
            }
            else
            {
                imageInfoNormal.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfoNormal.imageView = defaultTexture->mImageView;
                imageInfoNormal.sampler = defaultTexture->mSampler;
            }

            // IBL environment map
            VkDescriptorImageInfo imageInfoEnv{};
            imageInfoEnv.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            CubeTexture* envTexture = simpleScene->getEnvTexture();
            imageInfoEnv.imageView = envTexture->mImageView;
            imageInfoEnv.sampler = envTexture->mSampler;

            // IBL irradiance map
            VkDescriptorImageInfo imageInfoEnvIrr{};
            imageInfoEnvIrr.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

            CubeTexture* envIrrTexture = simpleScene->getEnvIrrTexture();
            imageInfoEnvIrr.imageView = envIrrTexture->mImageView;
            imageInfoEnvIrr.sampler = envIrrTexture->mSampler;

            // IBL LUT map
            VkDescriptorImageInfo imageInfoLUT{};
            imageInfoLUT.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            GLITexture* LUTTexture = simpleScene->getEnvLUTTexture();
            imageInfoLUT.imageView = LUTTexture->mImageView;
            imageInfoLUT.sampler = LUTTexture->mSampler;

            std::array<VkWriteDescriptorSet, 7> writeDescriptors;

            writeDescriptors[0] = VulkanHelper::bufferWriteDescriptorSet(ds, 0, &bufferInfo);
            writeDescriptors[1] = VulkanHelper::imageSamplerWriteDescriptorSet(ds, 1, &imageInfoAlbedo);
            writeDescriptors[2] = VulkanHelper::imageSamplerWriteDescriptorSet(ds, 2, &imageInfoMeR);
            writeDescriptors[3] = VulkanHelper::imageSamplerWriteDescriptorSet(ds, 3, &imageInfoNormal);
            writeDescriptors[4] = VulkanHelper::imageSamplerWriteDescriptorSet(ds, 4, &imageInfoEnv);
            writeDescriptors[5] = VulkanHelper::imageSamplerWriteDescriptorSet(ds, 5, &imageInfoEnvIrr);
            writeDescriptors[6] = VulkanHelper::imageSamplerWriteDescriptorSet(ds, 6, &imageInfoLUT);


            size_t writeDescriptorCount = writeDescriptors.size();
            VkWriteDescriptorSet* writeDescriptorData = writeDescriptors.data();

            vkUpdateDescriptorSets(mDevice->getLogicalDevice(), static_cast<uint32_t>(writeDescriptorCount), writeDescriptorData, 0, nullptr);
        }
    }
}

void PBRConstantBuffer::createUniformBuffers()
{
    VkDeviceSize bufferSize = sizeof(MVPCameraPosConstantBuffer);

    mUniformBuffers.resize(mMaxFramesInFligt);
    mUniformBuffersMemory.resize(mMaxFramesInFligt);
    mMappedData.resize(mMaxFramesInFligt);

    for (size_t i = 0; i < mMaxFramesInFligt; i++)
    {
        mDevice->createBuffer(bufferSize,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            mUniformBuffers[i],
            mUniformBuffersMemory[i]);

        vkMapMemory(mDevice->getLogicalDevice(),
            mUniformBuffersMemory[i],
            0,
            bufferSize,
            0,
            &mMappedData[i]);
    }
}