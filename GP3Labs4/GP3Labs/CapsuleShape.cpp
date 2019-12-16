#include "pch.h"
#include "CapsuleShape.h"
CapsuleShape::CapsuleShape(glm::float32 radius, glm::float32 height)
{
	m_radius = radius;
	m_height = height;
	m_shape = new btCapsuleShape(btScalar(m_radius), btScalar(m_height));// bullet function for capsule shape
	m_type = CShapeType::CAPSULE;
}
