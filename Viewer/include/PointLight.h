#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight(const glm::vec3& position);
	PointLight(const glm::vec3& position, const glm::vec3& color);
	PointLight();
	virtual ~PointLight();
	glm::vec3& GetPosition();

	glm::vec3 ambient_ref = glm::vec3(1);
	glm::vec3 diffuse_ref = glm::vec3(1);
	glm::vec3 specular_ref = glm::vec3(1);
	float alfa = 1;

private:
	glm::vec3 position = glm::vec3(0);
	
};
