#pragma once
#include <map>
#include <string>
#include <vector>

class RenderTask;
class RenderTaskResource;

class RenderTaskLinkInfo
{

	enum class LinkType : int
	{
		LINK_TYPE_TEXTURE,
		LINK_TYPE_BUFFER
	};

	enum class LinkFormat : int
	{
		LINK_FORMAT_D32,
		LINK_FORMAT_D24
	};

public:
	RenderTaskLinkInfo(RenderTask* srcPass,
		RenderTask* dstPass,
		LinkType type,
		LinkFormat format);
	virtual ~RenderTaskLinkInfo();

public:
	RenderTask* mSrcPass = nullptr;
	RenderTask* mDstPass = nullptr;
	LinkType mLinkType = LinkType::LINK_TYPE_TEXTURE;
	LinkFormat mLinkFormat = LinkFormat::LINK_FORMAT_D32;
};