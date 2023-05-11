#include "Mesh.h"

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
}