#include "Scene.h"
#include "BoundingBox.h"
Scene::Scene(Device* device)
    :mDevice(device)
{
    mBBox = new BoundingBox();
}

Scene::~Scene()
{
    delete mBBox;
}