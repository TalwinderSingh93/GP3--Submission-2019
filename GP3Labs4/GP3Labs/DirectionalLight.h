#pragma once
#include "pch.h"

class DirectionalLight
{
public:
	glm::vec3 lightDir;

	void AddLightDirection(glm::vec3 lightDirection);
	glm::vec3 GetLightDirection();

};

