#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BoundingBox.h"

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
	const void* vBufferData,
	uint32_t indices,
	VkDeviceSize iBufferSize,
	const void* iBbufferData,
	const BoundingBox* bbox,
	unsigned int materialIndex)
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

	if (bbox != nullptr)
	{
		mBBox = new BoundingBox(*bbox);
	}

	mMaterialIndex = materialIndex;
}