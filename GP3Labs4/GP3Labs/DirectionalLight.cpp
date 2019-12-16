#include "DirectionalLight.h"

void DirectionalLight::AddLightDirection(glm::vec3 lightDirection)
{
	lightDir = lightDirection;
}
glm::vec3 DirectionalLight::GetLightDirection()
{
	return lightDir;
}
