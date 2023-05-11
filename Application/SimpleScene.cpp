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
    for (size_t i = 0; i < mMeshes.size(); ++i)
    {
        Mesh* mesh = mMeshes[i];
        delete mesh;
    }
}

void SimpleScene::init()
{
    TriangleMesh* mesh;
    mesh = new TriangleMesh(mDevice);
    mMeshes.push_back(mesh);
}

Mesh* SimpleScene::getMesh(int index)
{
    if (index < 0 || index >(int)mMeshes.size())
    {
        throw std::runtime_error("mesh index out of range!");
    }
    return mMeshes[index];
}