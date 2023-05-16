#include "PCVertexFormat.h"


PCVertexFormat::PCVertexFormat()
{
    
}

PCVertexFormat::~PCVertexFormat()
{
 
}

void PCVertexFormat::init()
{
    mBinding.binding = 0;
    mBinding.stride = sizeof(Vertex);
    mBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    mAttribute.resize(2);
    mAttribute[0].binding = 0;
    mAttribute[0].location = 0;
    mAttribute[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    mAttribute[0].offset = offsetof(Vertex, pos);

    mAttribute[1].binding = 0;
    mAttribute[1].location = 1;
    mAttribute[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    mAttribute[1].offset = offsetof(Vertex, color);
}