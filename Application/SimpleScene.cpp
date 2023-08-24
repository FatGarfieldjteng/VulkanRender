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
#include "VulkanHelper.h"
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
    //createBox();
    loadScene();
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
    texture->load("D:/tengj/git/VulkanRender/Asset/viking_room.png", true);
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
    texture->load("../../Asset/viking_room.png", true);
    mTextures.push_back(texture);
}

void SimpleScene::loadGLTFScene()
{
    const aiScene* scene = aiImportFile("../../Asset/ABeautifulGame/glTF/ABeautifulGame.gltf", aiProcess_Triangulate| aiProcess_PreTransformVertices);

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

            vertices.push_back(vertex);
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
        
        pbrMaterial->mValues.mMapOrValue = 0;

        // Albedo
        if (material->GetTexture(AI_MATKEY_BASE_COLOR_TEXTURE, &mapBaseColor) == AI_SUCCESS)
        {
            texture = new Texture(mDevice);

            std::string file = strTextureFileBase + mapBaseColor.C_Str();
            texture->load(file.c_str(), true);
            mTextures.push_back(texture);

            pbrMaterial->mTextures.mAlbedo = texture;
            pbrMaterial->mValues.mMapOrValue |= PBRMaterial::HAS_ALBEDO_MAP;
        }
        
        if (material->Get(AI_MATKEY_BASE_COLOR, baseColorFactor) == AI_SUCCESS)
        {
            pbrMaterial->mValues.mAlbedo = glm::vec4(baseColorFactor.r, baseColorFactor.g, baseColorFactor.b, baseColorFactor.a);
        }

        // MetallicRoughness
        if (material->GetTexture(AI_MATKEY_METALLIC_TEXTURE, &mapMetallicRoughness) == AI_SUCCESS)
        {
            texture = new Texture(mDevice);
            std::string file = strTextureFileBase + mapMetallicRoughness.C_Str();
            texture->load(file.c_str(), false);
            mTextures.push_back(texture);

            pbrMaterial->mTextures.mMetalRoughness = texture;
            pbrMaterial->mValues.mMapOrValue |= PBRMaterial::HAS_ROUGHNESS_METALLIC_MAP;
        }

        if (material->Get(AI_MATKEY_METALLIC_FACTOR, metallicFactor) == AI_SUCCESS)
        {
            pbrMaterial->mValues.mMetallic = metallicFactor;
        }

        if (material->Get(AI_MATKEY_ROUGHNESS_FACTOR, roughnessFactor) == AI_SUCCESS)
        {
            pbrMaterial->mValues.mRoughness = roughnessFactor;
        }

        // Normal map
        if (material->Get(AI_MATKEY_TEXTURE(aiTextureType_NORMALS, 0), mapNormal) == AI_SUCCESS)
        {
            texture = new Texture(mDevice);
            std::string file = strTextureFileBase + mapNormal.C_Str();
            texture->load(file.c_str(), false);
            mTextures.push_back(texture);

            pbrMaterial->mTextures.mNormal = texture;
            pbrMaterial->mValues.mMapOrValue |= PBRMaterial::HAS_NORMAL_MAP;
        }

        mPBRMaterials.push_back(pbrMaterial);
    }
    
    loadIBLTextures();

    loadDefaultTexture();

    updateBBox();
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

void SimpleScene::loadDefaultTexture()
{
    std::string strTextureFileBase("../../Asset/");

    mDefaultTexture = new Texture(mDevice);

    std::string file = strTextureFileBase + "Default.jpg";
    mDefaultTexture->load(file.c_str(), false);
    mTextures.push_back(mDefaultTexture);
}

void SimpleScene::updateBBox()
{
    for (size_t meshIndex = 0; meshIndex < mMeshes.size(); ++meshIndex)
    {
        mBBox->update(mMeshes[meshIndex]->getBBox());
    }
}

int SimpleScene::getMeshCount()
{
    return static_cast<int>(mMeshes.size());
}

int SimpleScene::getMaterialCount()
{
    return static_cast<int>(mPBRMaterials.size());
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