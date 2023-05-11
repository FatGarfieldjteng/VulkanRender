#include "TriangleMesh.h"
#include "TriangleVertexBuffer.h"

TriangleMesh::TriangleMesh(Device* device)
    :Mesh(device)
{
    init();
}

TriangleMesh::~TriangleMesh()
{
 
}

void TriangleMesh::init()
{
    mVertexBuffer = new TriangleVertexBuffer(mDevice);
}