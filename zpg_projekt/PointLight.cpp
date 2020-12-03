#include "PointLight.hpp"


PointLight::PointLight(glm::vec3 position, Model model, glm::vec3 modelColor, GLuint shaderID, glm::vec3 lightColor, ShaderType shaderType, std::shared_ptr<Camera> camera) : Light(position, model, modelColor, shaderID, lightColor, shaderType, camera)
{
	this->lightType = POINT_LIGHT;
}