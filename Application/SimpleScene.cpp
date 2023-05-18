#include "SimpleScene.h"
#include "Mesh.h"
#include "Texture.h"
#include "PCVertexFormat.h"
#include "PCTVertexFormat.h"
#include <unordered_map>
#include <stdexcept>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"

SimpleScene::SimpleScene(Device* device)
    :Scene(device)
{
    init();
    loadScene();
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

}

void SimpleScene::init()
{
    /*TriangleMesh* mesh;
    mesh = new TriangleMesh(mDevice);*/

    const std::vector<PCTVertexFormat::Vertex> vertices =
    {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
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

    mesh->init(vertexCount,
        vBufferSize,
        (void*)vertices.data(),
        indexCount,
        iBufferSize,
        (void*)indices.data()
    );

    mMeshes.push_back(mesh);


    Texture* texture = new Texture(mDevice);
    texture->load("../../Asset/viking_room.png");
    mTextures.push_back(texture);
}

void SimpleScene::loadScene()
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

        for (const auto& index : shape.mesh.indices) 
        {
            PCTVertexFormat::Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.color = { 1.0f, 0.5f, 1.0f };

            vertex.texCoord = 
            {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0 - attrib.texcoords[2 * index.texcoord_index + 1]
             };

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
            (void*)indices.data()
        );

        mMeshes.push_back(mesh);
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
        throw std::runtime_error("mesh index out of range!");
    }
    return mTextures[index];
}