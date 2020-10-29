#include "ObjectFactory.hpp"
#include "Shader.hpp"
#include "ModelPrefabs.hpp"

Object ObjectFactory::createObject(std::string modelName, ShaderType shaderType, std::shared_ptr<Camera> camera)
{
	GLuint shaderProgram = Shader::createShader(shaderType, 2);
	ModelPrefabs modelPrefabs;
	Model model = modelPrefabs.getModel(modelName);

	return Object(DEFAULT_POSITION, model, DEFAULT_COLOR, shaderProgram, shaderType, camera);
}

Object ObjectFactory::createObject(std::string modelName, ShaderType shaderType, glm::vec3 position, glm::vec3 color, std::shared_ptr<Camera> camera)
{
	GLuint shaderProgram = Shader::createShader(shaderType, 2);
	ModelPrefabs modelPrefabs;
	Model model = modelPrefabs.getModel(modelName);

	return Object(position, model, color, shaderProgram, shaderType, camera);
}
