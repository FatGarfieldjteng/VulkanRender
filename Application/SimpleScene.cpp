#include "SimpleScene.h"
#include "TriangleMesh.h"
#include <stdexcept>

SimpleScene::SimpleScene(Device* device)
    :Scene(device)
{
    init();
}

SimpleScene::~SimpleScene()
{
 
}

void SimpleScene::init()
{
    TriangleMesh* mesh;
    mesh = new TriangleMesh(mDevice);
    meshes.push_back(mesh);
}

Mesh* SimpleScene::getMesh(int index)
{
    if (index < 0 || index >(int)meshes.size())
    {
        throw std::runtime_error("mesh index out of range!");
    }
    return meshes[index]; 
}