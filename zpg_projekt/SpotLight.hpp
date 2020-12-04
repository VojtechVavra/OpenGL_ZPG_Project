#pragma once

//#include <glm/vec3.hpp>
#include "PointLight.hpp"

class SpotLight : public PointLight
{
public:
	SpotLight(glm::vec3 position, Model model, glm::vec3 modelColor, GLuint shaderProgram, ShaderType shaderType, glm::vec3 lightColor, float attenuation, float ambientCoefficient, float coneAngle, glm::vec3 coneDirection);
	SpotLight(glm::vec3 position, GLuint shaderProgram, ShaderType shaderType, glm::vec3 lightColor, float attenuation, float ambientCoefficient, float coneAngle, glm::vec3 coneDirection);

	glm::vec3 coneDirection;
private:
	float coneAngle;
	
};

