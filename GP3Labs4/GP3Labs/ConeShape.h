#pragma once
#include "pch.h"
#include "CollisionShape.h"
class ConeShape : public CollisionShape
{
private:
	glm::float32 m_radius;
	glm::float32 m_height;
public:
	ConeShape(glm::float32 radius, glm::float32 height);
};