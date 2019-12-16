#pragma once
#include "pch.h"
#include "CollisionShape.h"
class SphereShape : public CollisionShape
{
private:
	glm::float32 m_radius;
public:
	SphereShape(glm::float32 radius);
};
