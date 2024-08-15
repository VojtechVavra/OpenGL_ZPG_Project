#include "DirectionalLight.hpp"


DirectionalLight::DirectionalLight(glm::vec3 position/*, Model model*/, glm::vec3 modelColor, GLuint shaderProgram, ShaderType shaderType, glm::vec3 lightColor, glm::vec3 direction, float ambientCoefficient) : Light(position/*, model*/, modelColor, shaderProgram, shaderType, lightColor, ambientCoefficient)
{
	this->lightType = LightType::DIRECTION_LIGHT;
	this->direction = direction;
}

DirectionalLight::DirectionalLight(GLuint shaderProgram, ShaderType shaderType, glm::vec3 lightColor, glm::vec3 direction, float ambientCoefficient) : Light(glm::vec3(0.0f, 0.0f, 0.0f), shaderProgram, shaderType, lightColor, ambientCoefficient)
{
	this->lightType = LightType::DIRECTION_LIGHT;
	this->direction = direction;
}
