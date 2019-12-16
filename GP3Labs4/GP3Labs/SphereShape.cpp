#include "pch.h"
#include "SphereShape.h"
SphereShape::SphereShape(glm::float32 radius)
{
	m_radius = radius;
	m_shape = new btSphereShape(btScalar(m_radius));// bullet function for sphere shape
	m_type = CShapeType::SPHERE;
}
