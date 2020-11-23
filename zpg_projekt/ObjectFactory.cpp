#include "ObjectFactory.hpp"
#include "Shader.hpp"
#include "ModelPrefabs.hpp"

std::shared_ptr<Object> ObjectFactory::createObject(std::string modelName, ShaderType shaderType)
{
	GLuint shaderProgram = Shader::createShader(shaderType, 2);
	ModelPrefabs modelPrefabs;
	Model model = modelPrefabs.getModel(modelName);

	return std::shared_ptr<Object>(new Object(DEFAULT_POSITION, model, DEFAULT_COLOR, shaderProgram, shaderType));
}

std::shared_ptr<Object> ObjectFactory::createObject(std::string modelName, ShaderType shaderType, glm::vec3 position, glm::vec3 color)
{
	// oddelit objekt od shaderu. shadery se budou registrovat na kameru
	GLuint shaderProgram = Shader::createShader(shaderType, 2);
	ModelPrefabs modelPrefabs;
	Model model = modelPrefabs.getModel(modelName);

	return std::shared_ptr<Object>(new Object(position, model, color, shaderProgram, shaderType));
}
