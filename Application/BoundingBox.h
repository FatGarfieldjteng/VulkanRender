#pragma once
#include <glm/glm.hpp>
#include <limits>

class BoundingBox
{
public:
	BoundingBox();
	virtual ~BoundingBox();

public:
	void update(const glm::vec3& pos);
	void update(const BoundingBox* bbox);
	glm::vec3 center();
	glm::vec3 extent();
public:
	glm::vec3 mMinCorner = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 mMaxCorner = glm::vec3(std::numeric_limits<float>::lowest());
};