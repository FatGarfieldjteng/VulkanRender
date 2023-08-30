#pragma once

class RenderTaskResource
{
public:
	enum class TYPE
	{
		TYPE_DEPTH,
		TTPE_COLOR
	};

	RenderTaskResource();
	virtual ~RenderTaskResource();

	enum class TYPE getType()
	{
		return mType;
	}

protected:
	enum class TYPE mType = TYPE::TYPE_DEPTH;
};