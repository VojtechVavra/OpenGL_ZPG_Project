#include "PointLight.hpp"


PointLight::PointLight(glm::vec3 position, Model model, glm::vec3 modelColor, GLuint shaderProgram, ShaderType shaderType, glm::vec3 lightColor, float attenuation, float ambientCoefficient) : Light(position, model, modelColor, shaderProgram, shaderType, lightColor, ambientCoefficient)
{
	this->lightType = LightType::POINT_LIGHT;
	this->attenuation = attenuation;
	//this->ambientCoefficient = ambientCoefficient;
}

PointLight::PointLight(glm::vec3 position, GLuint shaderProgram, ShaderType shaderType, glm::vec3 lightColor, float attenuation, float ambientCoefficient) : Light(position, shaderProgram, shaderType, lightColor, ambientCoefficient)
{
	this->lightType = LightType::POINT_LIGHT;
	this->attenuation = attenuation;
	//this->ambientCoefficient = ambientCoefficient;
}