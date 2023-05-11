#include "SimpleScene.h"
#include "TriangleMesh.h"

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
}