#include "SpotLight.hpp"

SpotLight::SpotLight(glm::vec3 position, Model model, glm::vec3 modelColor, GLuint shaderProgram, ShaderType shaderType, glm::vec3 lightColor, float attenuation, float ambientCoefficient, float coneAngle, glm::vec3 coneDirection) : PointLight(position, model, modelColor, shaderProgram, shaderType, lightColor, attenuation, ambientCoefficient)
{
	this->lightType = LightType::SPOT_LIGHT;
	this->coneAngle = coneAngle;
	this->coneDirection = coneDirection;
}

SpotLight::SpotLight(glm::vec3 position, GLuint shaderProgram, ShaderType shaderType, glm::vec3 lightColor, float attenuation, float ambientCoefficient, float coneAngle, glm::vec3 coneDirection) : PointLight(position, shaderProgram, shaderType, lightColor, attenuation, ambientCoefficient)
{
	this->lightType = LightType::SPOT_LIGHT;
	this->coneAngle = coneAngle;
	this->coneDirection = coneDirection;
}
