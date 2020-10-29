#ifndef OBJECTFACTORY_HPP
#define OBJECTFACTORY_HPP

#include <memory>

#include <glm/vec3.hpp>

#include "Object.hpp"
#include "Camera.hpp"

class ObjectFactory
{
public:
	//std::unique_ptr<Object> createObject(std::string modelName, ShaderType shaderType);
	Object createObject(std::string modelName, ShaderType shaderType, std::shared_ptr<Camera> camera);
	Object createObject(std::string modelName, ShaderType shaderType, glm::vec3 position, glm::vec3 color, std::shared_ptr<Camera> camera);
private:
	const glm::vec3 DEFAULT_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 DEFAULT_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);
};

#endif // !OBJECTFACTORY_HPP