#pragma once

#include "Light.hpp"

class DirectionalLight : public Light
{
public:
	DirectionalLight(glm::vec3 position, Model model, glm::vec3 modelColor, GLuint shaderProgram, ShaderType shaderType, glm::vec3 lightColor, glm::vec3 direction, float ambientCoefficient);
	DirectionalLight(GLuint shaderProgram, ShaderType shaderType, glm::vec3 lightColor, glm::vec3 direction, float ambientCoefficient);

	glm::vec3 direction;
private:
	
};

