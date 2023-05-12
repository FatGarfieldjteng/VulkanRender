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

void TriangleMesh::init(uint32_t /*vertices*/,
	VkDeviceSize /*vBufferSize*/,
	void* /*vBufferData*/,
	uint32_t /*indices*/,
	VkDeviceSize /*iBufferSize*/,
	void* /*iBbufferData*/)
{
    mVertexBuffer = new TriangleVertexBuffer(mDevice);
}