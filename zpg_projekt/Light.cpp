#include "Light.hpp"

Light::Light(glm::vec3 position/*, Model model*/, glm::vec3 modelColor, GLuint shaderProgram, ShaderType shaderType, glm::vec3 lightColor, float ambientCoefficient) : Object(position/*, model, modelColor, shaderProgram, shaderType*/) {
	this->lightType = (LightType)-1;
	this->lightColor = lightColor;
	this->ambientCoefficient = ambientCoefficient;
	//this->lightIntensity = 1.0f;
}

Light::Light(glm::vec3 position, GLuint shaderProgram, ShaderType shaderType, glm::vec3 lightColor, float ambientCoefficient) : Object(position/*, shaderProgram, shaderType*/)
{
	this->lightType = (LightType)-1;
	this->ambientCoefficient = ambientCoefficient;
	this->lightColor = lightColor;
}

/*Light::Light() {
}
Light::~Light() {
}*/

/*void Light::render() {
	//glBindVertexArray(VAO);
}*/

Light::LightType Light::getLightType()
{
	return this->lightType;
	//return Light::LightType(-1); // this->lightType;
}