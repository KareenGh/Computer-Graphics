#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>

/*Defining a new class for Light*/
class Light
{
public:
	Light() {}
	glm::mat4x4 TranslateMat = glm::mat4x4(1);
	glm::vec3 ambient_ref = glm::vec3(1);
	glm::vec3 diffuse_ref = glm::vec3(1);
	glm::vec3 specular_ref = glm::vec3(1);
};

