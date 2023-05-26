#include "BoundingBox.h"

BoundingBox::BoundingBox()
{

}

BoundingBox::~BoundingBox()
{

}

void BoundingBox::update(const glm::vec3& pos)
{
	mMinCorner.x = mMinCorner.x < pos.x ? mMinCorner.x : pos.x;
	mMinCorner.y = mMinCorner.y < pos.y ? mMinCorner.y : pos.y;
	mMinCorner.z = mMinCorner.z < pos.z ? mMinCorner.z : pos.z;

	mMaxCorner.x = mMaxCorner.x > pos.x ? mMaxCorner.x : pos.x;
	mMaxCorner.y = mMaxCorner.y > pos.y ? mMaxCorner.y : pos.y;
	mMaxCorner.z = mMaxCorner.z > pos.z ? mMaxCorner.z : pos.z;
}

void BoundingBox::update(const BoundingBox* bbox)
{
	update(bbox->mMinCorner);
	update(bbox->mMaxCorner);
}

glm::vec3 BoundingBox::center()
{
	return (mMinCorner + mMaxCorner) * 0.5f;
}

glm::vec3 BoundingBox::extent()
{
	return (mMaxCorner - mMinCorner);
}
