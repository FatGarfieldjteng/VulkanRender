#include "SimpleScene.h"
#include "Mesh.h"
#include "Texture.h"
#include "CubeTexture.h"
#include "GLITexture.h"
#include "PCVertexFormat.h"
#include "PCTVertexFormat.h"
#include "PBRVertexFormat.h"
#include "BoundingBox.h"
#include "PBRMaterial.h"
#include "Device.h"
#include <unordered_map>
#include <stdexcept>
#include <array>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/version.h>
#include <assimp/pbrmaterial.h>
#include <assimp/material.h>

SimpleScene::SimpleScene(Device* device)
    :Scene(device)
{
    init();
}

SimpleScene::~SimpleScene()
{
    for (size_t i = 0; i < mTextures.size(); ++i)
    {
        Texture* texture = mTextures[i];
        delete texture;
    }

    for (size_t i = 0; i < mMeshes.size(); ++i)
    {
        Mesh* mesh = mMeshes[i];
        delete mesh;
    }

    for (size_t i = 0; i < mPBRMaterials.size(); ++i)
    {
        PBRMaterial* mesh = mPBRMaterials[i];
        delete mesh;
    }
}

void SimpleScene::init()
{
    createBox();
    //loadScene();
}

void SimpleScene::createBox()
{
    /*TriangleMesh* mesh;
    mesh = new TriangleMesh(mDevice);*/

    const std::vector<PCTVertexFormat::Vertex> vertices =
    {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},

        /*{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},*/

        /*{{-0.5f, -0.5f, -0.5f}, {0.5f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 0.5f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 0.5}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}}*/
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };

    const std::vector<uint32_t> indices = 
    {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4
    };

    // QUAD mesh
    Mesh* mesh = new Mesh(mDevice);

    uint32_t vertexCount = static_cast<uint32_t>(vertices.size());
    VkDeviceSize vBufferSize = static_cast< VkDeviceSize>(sizeof(PCTVertexFormat::Vertex) * vertexCount);

    uint32_t indexCount = static_cast<uint32_t>(indices.size());
    VkDeviceSize iBufferSize = static_cast <VkDeviceSize>(sizeof(uint32_t) * indexCount);

    // compute bounding box
    BoundingBox bbox;

    for (size_t vertexIndex = 0; vertexIndex < vertices.size(); ++vertexIndex)
    {
        bbox.update(vertices[vertexIndex].pos);
    }

    mesh->init(vertexCount,
        vBufferSize,
        (void*)vertices.data(),
        indexCount,
        iBufferSize,
        (void*)indices.data(),
        &bbox
    );

    mMeshes.push_back(mesh);

    updateBBox();

    Texture* texture = new Texture(mDevice);
    texture->load("E:/PersonalGit/VulkanRender/Asset/viking_room.png");
    mTextures.push_back(texture);
}

void SimpleScene::loadScene()
{
    //loadObjScene();
    loadGLTFScene();
}

void SimpleScene::loadObjScene()
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, "../../Asset/viking_room.obj")) {
        throw std::runtime_error(warn + err);
    }

    std::unordered_map<PCTVertexFormat::Vertex, uint32_t> uniqueVertices{};

    for (const auto& shape : shapes) {

        std::vector<uint32_t> indices;
        std::vector<PCTVertexFormat::Vertex> vertices;

        BoundingBox bbox;

        for (const auto& index : shape.mesh.indices)
        {
            PCTVertexFormat::Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            bbox.update(vertex.pos);

            vertex.color = { 1.0f, 0.5f, 1.0f };

            if (index.texcoord_index >= 0)
            {
                vertex.texCoord =
                {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0 - attrib.texcoords[2 * index.texcoord_index + 1]
                };
            }

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }

        Mesh* mesh = new Mesh(mDevice);

        uint32_t vertexCount = static_cast<uint32_t>(vertices.size());
        VkDeviceSize vBufferSize = static_cast<VkDeviceSize>(sizeof(PCTVertexFormat::Vertex) * vertexCount);

        uint32_t indexCount = static_cast<uint32_t>(indices.size());
        VkDeviceSize iBufferSize = static_cast <VkDeviceSize>(sizeof(uint32_t) * indexCount);

        mesh->init(vertexCount,
            vBufferSize,
            (void*)vertices.data(),
            indexCount,
            iBufferSize,
            (void*)indices.data(),
            &bbox
        );

        mMeshes.push_back(mesh);
    }

    updateBBox();

    Texture* texture = new Texture(mDevice);
    texture->load("../../Asset/viking_room.png");
    mTextures.push_back(texture);
}

void SimpleScene::loadGLTFScene()
{
    const aiScene* scene = aiImportFile("../../Asset/ABeautifulGame/glTF/ABeautifulGame.gltf", aiProcess_Triangulate);

    if (!scene || !scene->HasMeshes())
    {
        throw std::runtime_error("Can't find ABeautifulGame.gltf");
    }

    for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
    {
        aiMesh* originalMesh = scene->mMeshes[meshIndex];

        if (!originalMesh->HasPositions() ||
            !originalMesh->HasFaces() )
        {
            continue;
        }
        
        std::vector<PBRVertexFormat::Vertex> vertices;
        vertices.reserve(originalMesh->mNumVertices);
        std::vector<uint32_t> indices;
        indices.reserve(originalMesh->mNumFaces * 3);
        BoundingBox bbox;

        for (unsigned vertexIndex = 0; vertexIndex < originalMesh->mNumVertices; ++vertexIndex)
        {
            PBRVertexFormat::Vertex vertex{};

            const aiVector3D v = originalMesh->mVertices[vertexIndex];
            vertex.pos = { v.x, v.y, v.z };

            bbox.update(vertex.pos);

            if (originalMesh->HasNormals())
            {
                const aiVector3D n = originalMesh->mNormals[vertexIndex];
                vertex.normal = {n.x, n.y, n.z};
            }

            if (originalMesh->HasTextureCoords(0))
            {
                const aiVector3D t = originalMesh->mTextureCoords[0][vertexIndex];
                vertex.texCoord = {t.x, 1.0f - t.y};
            }
        }

        for (unsigned int faceIndex = 0; faceIndex < originalMesh->mNumFaces; ++faceIndex)
        {
            aiFace &face = originalMesh->mFaces[faceIndex];
            indices.push_back(face.mIndices[0]);
            indices.push_back(face.mIndices[1]);
            indices.push_back(face.mIndices[2]);
        }

        Mesh* mesh = new Mesh(mDevice);

        uint32_t vertexCount = static_cast<uint32_t>(vertices.size());
        VkDeviceSize vBufferSize = static_cast<VkDeviceSize>(sizeof(PBRVertexFormat::Vertex) * vertexCount);

        uint32_t indexCount = static_cast<uint32_t>(indices.size());
        VkDeviceSize iBufferSize = static_cast <VkDeviceSize>(sizeof(uint32_t) * indexCount);

        mesh->init(vertexCount,
            vBufferSize,
            (void*)vertices.data(),
            indexCount,
            iBufferSize,
            (void*)indices.data(),
            &bbox,
            originalMesh->mMaterialIndex
        );
       
        mMeshes.push_back(mesh);

    } // for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)

    aiReturn result;

    Texture* texture = nullptr;
    PBRMaterial* pbrMaterial = nullptr;
    
    std::string strTextureFileBase("../../Asset/ABeautifulGame/glTF/");

    for (unsigned int materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex)
    {
        aiString mapBaseColor, mapMetallicRoughness, mapNormal;
        aiColor4D baseColorFactor;
        ai_real metallicFactor, roughnessFactor;

        aiMaterial* material = scene->mMaterials[materialIndex];

        pbrMaterial = new PBRMaterial();
        
        // Albedo
        if (material->GetTexture(AI_MATKEY_BASE_COLOR_TEXTURE, &mapBaseColor) == AI_SUCCESS)
        {
            texture = new Texture(mDevice);

            std::string file = strTextureFileBase + mapBaseColor.C_Str();
            texture->load(file.c_str());
            mTextures.push_back(texture);

            pbrMaterial->mTexAlbedo = texture;
        }
        
        if (material->Get(AI_MATKEY_BASE_COLOR, baseColorFactor) == AI_SUCCESS)
        {
            pbrMaterial->mAlbedoFactor = glm::vec4(baseColorFactor.r, baseColorFactor.g, baseColorFactor.b, baseColorFactor.a);
        }

        // MetallicRoughness
        if (material->GetTexture(AI_MATKEY_METALLIC_TEXTURE, &mapMetallicRoughness) == AI_SUCCESS)
        {
            texture = new Texture(mDevice);
            std::string file = strTextureFileBase + mapMetallicRoughness.C_Str();
            texture->load(file.c_str());
            mTextures.push_back(texture);

            pbrMaterial->mTexMetalRoughness = texture;
        }

        if (material->Get(AI_MATKEY_METALLIC_FACTOR, metallicFactor) == AI_SUCCESS)
        {
            pbrMaterial->mMetalRoughnessFactor = metallicFactor;
        }

        // Normal map
        if (material->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), mapNormal) == AI_SUCCESS)
        {
            texture = new Texture(mDevice);
            std::string file = strTextureFileBase + mapNormal.C_Str();
            texture->load(file.c_str());
            mTextures.push_back(texture);

            pbrMaterial->mTexNormal = texture;
        }

        mPBRMaterials.push_back(pbrMaterial);
    }
    
    loadIBLTextures();

    updateBBox();

    createUniformBuffers();

    createPBRDescriptorLayout();

    createDescriptorPool(mPBRMaterials.size(), 1, 6, 2);

    createDescriptorSet(2);
}

void SimpleScene::loadIBLTextures()
{
    std::string strTextureFileBase("../../Asset/");

    mEnvTexture = new CubeTexture(mDevice);
    std::string strEnvTexture = strTextureFileBase + "piazza_bologni_1k.hdr";
    mEnvTexture->load(strEnvTexture.c_str());

    mEnvIrrTexture = new CubeTexture(mDevice);
    std::string strEnvIrrTexture = strTextureFileBase + "piazza_bologni_1k_irradiance.hdr";
    mEnvIrrTexture->load(strEnvIrrTexture.c_str());

    mLUTTexture = new GLITexture(mDevice);
    std::string strLUTTexture = strTextureFileBase + "brdfLUT.ktx";
    mLUTTexture->load(strLUTTexture.c_str());
}

void SimpleScene::updateBBox()
{
    for (size_t meshIndex = 0; meshIndex < mMeshes.size(); ++meshIndex)
    {
        mBBox->update(mMeshes[meshIndex]->getBBox());
    }
}

void SimpleScene::createUniformBuffers(int frameInFlight)
{
    VkDeviceSize bufferSize = sizeof(MVPCameraPosConstantBuffer);

    mUniformBuffers.resize(frameInFlight);
    mUniformBuffersMemory.resize(frameInFlight);
    mMappedData.resize(frameInFlight);

    for (size_t i = 0; i < frameInFlight; i++)
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

void SimpleScene::createPBRDescriptorLayout()
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

    vkCreateDescriptorSetLayout(mDevice->getLogicalDevice(), &layoutInfo, nullptr, &mPBRDescriptorSetLayout);
}

void SimpleScene::createDescriptorPool(int materialCount, int uniformBufferCount, int samplerCount, int frameInFlight)
{
    int materials = (int)mPBRMaterials.size();

    std::vector<VkDescriptorPoolSize> poolSizes;

    if(uniformBufferCount > 0)
    {
        VkDescriptorPoolSize uniformPoolSize{};
        uniformPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uniformPoolSize.descriptorCount = frameInFlight * materialCount * uniformBufferCount;
        poolSizes.push_back(uniformPoolSize);
    }

    if (samplerCount > 0)
    {
        VkDescriptorPoolSize samplerPoolSize{};
        samplerPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        samplerPoolSize.descriptorCount = frameInFlight * materialCount * samplerCount;
        poolSizes.push_back(samplerPoolSize);
    }

    VkDescriptorPoolCreateInfo poolCreateInfo{};
    poolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolCreateInfo.pNext = nullptr;
    poolCreateInfo.flags = 0;
    poolCreateInfo.maxSets = static_cast<uint32_t>(frameInFlight * materialCount),
    poolCreateInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolCreateInfo.pPoolSizes = poolSizes.empty() ? nullptr : poolSizes.data();

    vkCreateDescriptorPool(mDevice->getLogicalDevice(), &poolCreateInfo, nullptr, &mDescriptorPool);

}

void SimpleScene::createDescriptorSet(int frameInFlight)
{
    int materials = (int)mPBRMaterials.size();

    std::vector<VkDescriptorSetLayout> layouts(frameInFlight * materials, mPBRDescriptorSetLayout);

    VkDescriptorSetAllocateInfo allocInfo{};

    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.pNext = nullptr;
    allocInfo.descriptorPool = mDescriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(frameInFlight * materials);
    allocInfo.pSetLayouts = layouts.data();

    mDescriptorSets.resize(frameInFlight * materials);

    // allocate descriptor sets
    vkAllocateDescriptorSets(mDevice->getLogicalDevice(), &allocInfo, mDescriptorSets.data());

    for (int frameIndex = 0; frameIndex < frameInFlight; ++frameIndex)
    {
        for (int materialIndex = 0; materialIndex < materials; ++materials)
        {
            int descriptorIndex = frameIndex * materials + materialIndex;
            VkDescriptorSet ds = mDescriptorSets[descriptorIndex];

            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = mUniformBuffers[frameIndex];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(MVPCameraPosConstantBuffer);

            PBRMaterial* mat = mPBRMaterials[materialIndex];

            VkDescriptorImageInfo imageInfoAlbedo{};
            imageInfoAlbedo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfoAlbedo.imageView = mat->mTexAlbedo->mImageView;
            imageInfoAlbedo.sampler = mat->mTexAlbedo->mSampler;

            VkDescriptorImageInfo imageInfoMeR{};
            imageInfoMeR.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfoMeR.imageView = mat->mTexMetalRoughness->mImageView;
            imageInfoMeR.sampler = mat->mTexMetalRoughness->mSampler;

            VkDescriptorImageInfo imageInfoNormal{};
            imageInfoNormal.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfoNormal.imageView = mat->mTexNormal->mImageView;
            imageInfoNormal.sampler = mat->mTexNormal->mSampler;

            VkDescriptorImageInfo imageInfoEnv{};
            imageInfoEnv.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfoEnv.imageView = mEnvTexture->mImageView;
            imageInfoEnv.sampler = mEnvTexture->mSampler;

            VkDescriptorImageInfo imageInfoEnvIrr{};
            imageInfoEnvIrr.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfoEnvIrr.imageView = mEnvIrrTexture->mImageView;
            imageInfoEnvIrr.sampler = mEnvIrrTexture->mSampler;

            VkDescriptorImageInfo imageInfoLUT{};
            imageInfoLUT.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfoLUT.imageView = mLUTTexture->mImageView;
            imageInfoLUT.sampler = mLUTTexture->mSampler;

            std::array<VkWriteDescriptorSet, 7> writeDescriptors;

            writeDescriptors[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptors[0].dstSet = ds;
            writeDescriptors[0].dstBinding = 0;
            writeDescriptors[0].dstArrayElement = 0;
            writeDescriptors[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            writeDescriptors[0].descriptorCount = 1;
            writeDescriptors[0].pBufferInfo = &bufferInfo;

            writeDescriptors[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptors[1].dstSet = ds;
            writeDescriptors[1].dstBinding = 1;
            writeDescriptors[1].dstArrayElement = 0;
            writeDescriptors[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            writeDescriptors[1].descriptorCount = 1;
            writeDescriptors[1].pImageInfo = &imageInfoAlbedo;

            writeDescriptors[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptors[2].dstSet = ds;
            writeDescriptors[2].dstBinding = 2;
            writeDescriptors[2].dstArrayElement = 0;
            writeDescriptors[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            writeDescriptors[2].descriptorCount = 1;
            writeDescriptors[2].pImageInfo = &imageInfoMeR;

            writeDescriptors[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptors[3].dstSet = ds;
            writeDescriptors[3].dstBinding = 3;
            writeDescriptors[3].dstArrayElement = 0;
            writeDescriptors[3].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            writeDescriptors[3].descriptorCount = 1;
            writeDescriptors[3].pImageInfo = &imageInfoNormal;

            writeDescriptors[4].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptors[4].dstSet = ds;
            writeDescriptors[4].dstBinding = 4;
            writeDescriptors[4].dstArrayElement = 0;
            writeDescriptors[4].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            writeDescriptors[4].descriptorCount = 1;
            writeDescriptors[4].pImageInfo = &imageInfoEnv;

            writeDescriptors[5].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptors[5].dstSet = ds;
            writeDescriptors[5].dstBinding = 5;
            writeDescriptors[5].dstArrayElement = 0;
            writeDescriptors[5].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            writeDescriptors[5].descriptorCount = 1;
            writeDescriptors[5].pImageInfo = &imageInfoEnvIrr;

            writeDescriptors[6].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            writeDescriptors[6].dstSet = ds;
            writeDescriptors[6].dstBinding = 6;
            writeDescriptors[6].dstArrayElement = 0;
            writeDescriptors[6].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            writeDescriptors[6].descriptorCount = 1;
            writeDescriptors[6].pImageInfo = &imageInfoLUT;

            vkUpdateDescriptorSets(mDevice->getLogicalDevice(), static_cast<uint32_t>(writeDescriptors.size()), writeDescriptors.data(), 0, nullptr);
        }
    }
}

int SimpleScene::getMeshCount()
{
    return static_cast<int>(mMeshes.size());
}

Mesh* SimpleScene::getMesh(int index)
{
    if (index < 0 || index >(int)mMeshes.size())
    {
        throw std::runtime_error("mesh index out of range!");
    }
    return mMeshes[index];
}

int SimpleScene::getTextureCount()
{
    return (int)mTextures.size();
}

Texture* SimpleScene::getTexture(int index)
{
    if (index < 0 || index >(int)mTextures.size())
    {
        throw std::runtime_error("texture index out of range!");
    }
    return mTextures[index];
}