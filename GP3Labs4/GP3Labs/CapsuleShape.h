#pragma once
#include "pch.h"
#include "CollisionShape.h"
class CapsuleShape : public CollisionShape
{
private:
	glm::float32 m_radius;
	glm::float32 m_height;
public:
	CapsuleShape(glm::float32 radius, glm::float32 height);
};