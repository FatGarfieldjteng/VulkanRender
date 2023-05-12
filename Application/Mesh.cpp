#include "Mesh.h"
#include "TriangleVertexBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

Mesh::Mesh(Device* device)
    :mDevice(device)
{

}

Mesh::~Mesh()
{
  if(mVertexBuffer)
  {
      delete mVertexBuffer;
  }

  if (mIndexBuffer)
  {
	  delete mIndexBuffer;
  }
}

void Mesh::init(uint32_t vertices,
	VkDeviceSize vBufferSize,
	void* vBufferData,
	uint32_t indices,
	VkDeviceSize iBufferSize,
	void* iBbufferData)
{
	if (vertices > 0 && vBufferSize > 0 && vBufferData)
	{
		mVertexBuffer = new VertexBuffer(mDevice);
		mVertexBuffer->init(vertices, vBufferSize, vBufferData);
	}

	if (indices > 0 && iBufferSize > 0 && iBbufferData)
	{
		mIndexBuffer = new IndexBuffer(mDevice);
		mIndexBuffer->init(indices, iBufferSize, iBbufferData);
	}
}