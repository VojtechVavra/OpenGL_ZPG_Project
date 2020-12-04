#ifndef OBJECTFACTORY_HPP
#define OBJECTFACTORY_HPP

#include <memory>

#include <glm/vec3.hpp>

#include "Object.hpp"
#include "Light.hpp"
#include "Camera.hpp"

class ObjectFactory
{
public:
	static std::shared_ptr<ObjectFactory> getInstance();

	std::shared_ptr<Object> createObject(std::string modelName, ShaderType shaderType);
	std::shared_ptr<Object> createObject(std::string modelName, ShaderType shaderType, glm::vec3 position, glm::vec3 color);

	
	// Point light
	std::shared_ptr<Light> createPointLight(std::string modelName, ShaderType shaderType, glm::vec3 position, glm::vec3 objectColor, glm::vec3 lightColor, float attenuation, float ambientCoefficient);
	// light without model
	std::shared_ptr<Light> createPointLight(glm::vec3 position, ShaderType shaderType, glm::vec3 lightColor, float attenuation, float ambientCoefficient);

	// Spot light
	std::shared_ptr<Light> createSpotLight(std::string modelName, ShaderType shaderType, glm::vec3 position, glm::vec3 objectColor, glm::vec3 lightColor, float attenuation, float ambientCoefficient, float coneAngle, glm::vec3 coneDirection);
	// light without model
	std::shared_ptr<Light> createSpotLight(glm::vec3 position, ShaderType shaderType, glm::vec3 lightColor, float attenuation, float ambientCoefficient, float coneAngle, glm::vec3 coneDirection);

	// Directional light
	std::shared_ptr<Light> createDirectionalLight(std::string modelName, ShaderType shaderType, glm::vec3 position, glm::vec3 objectColor, glm::vec3 lightColor, glm::vec3 direction, float ambientCoefficient);
	// light without model
	std::shared_ptr<Light> createDirectionalLight(ShaderType shaderType, glm::vec3 lightColor, glm::vec3 direction, float ambientCoefficient);

	
private:
	static std::shared_ptr<ObjectFactory> instance;
	ObjectFactory();
	const glm::vec3 DEFAULT_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 DEFAULT_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);
};

#endif // !OBJECTFACTORY_HPP