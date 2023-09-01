#include "RenderTaskLinkInfo.h"

RenderTaskLinkInfo::RenderTaskLinkInfo(RenderTask* srcPass,
	RenderTask* dstPass,
	LinkType type,
	LinkFormat format)
    :mLinkType(type),
    mLinkFormat(format)
{
    
}

RenderTaskLinkInfo::~RenderTaskLinkInfo()
{
	
}