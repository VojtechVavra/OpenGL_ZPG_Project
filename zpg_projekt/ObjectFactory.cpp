#include "ObjectFactory.hpp"
#include "Shader.hpp"
#include "ModelPrefabs.hpp"
#include "SpotLight.hpp"
#include "DirectionalLight.hpp"

std::shared_ptr<ObjectFactory> ObjectFactory::instance = nullptr;

std::shared_ptr<ObjectFactory> ObjectFactory::getInstance()
{
	if (ObjectFactory::instance == nullptr) {
		//ObjectFactory::instance = std::make_shared<ObjectFactory>();
		std::shared_ptr<ObjectFactory> inst(new ObjectFactory());
		ObjectFactory::instance = inst;
	}
	return ObjectFactory::instance;
}

ObjectFactory::ObjectFactory()
{

}

std::shared_ptr<Object> ObjectFactory::createObject(std::string modelName, ShaderType shaderType)
{
	GLuint shaderProgram = Shader::createShader(shaderType);	// ,2);
	auto modelPrefabs = ModelPrefabs::getInstance();
	Model model = modelPrefabs->getModel(modelName);

	return std::shared_ptr<Object>(new Object(DEFAULT_POSITION, model, DEFAULT_COLOR, shaderProgram, shaderType));
}

std::shared_ptr<Object> ObjectFactory::createObject(std::string modelName, ShaderType shaderType, glm::vec3 position, glm::vec3 color)
{
	// oddelit objekt od shaderu. shadery se budou registrovat na kameru
	GLuint shaderProgram = Shader::createShader(shaderType);
	auto modelPrefabs = ModelPrefabs::getInstance();
	Model model = modelPrefabs->getModel(modelName);

	return std::shared_ptr<Object>(new Object(position, model, color, shaderProgram, shaderType));
}


// Lights

// Point Light with model
std::shared_ptr<Light> ObjectFactory::createPointLight(std::string modelName, ShaderType shaderType, glm::vec3 position, glm::vec3 objectColor, glm::vec3 lightColor, float attenuation, float ambientCoefficient)
{
	GLuint shaderProgram = Shader::createShader(shaderType);
	auto modelPrefabs = ModelPrefabs::getInstance();
	Model lightModel = modelPrefabs->getModel(modelName);

	return std::make_shared<PointLight>(position, lightModel, objectColor, shaderProgram, shaderType, lightColor, attenuation, ambientCoefficient);
}
// Point Light without model
std::shared_ptr<Light> ObjectFactory::createPointLight(glm::vec3 position, ShaderType shaderType, glm::vec3 lightColor, float attenuation, float ambientCoefficient)
{
	GLuint shaderProgram = Shader::createShader(shaderType);

	return std::make_shared<PointLight>(position, shaderProgram, shaderType, lightColor, attenuation, ambientCoefficient);
}


// Spot Light with model
std::shared_ptr<Light> ObjectFactory::createSpotLight(std::string modelName, ShaderType shaderType, glm::vec3 position, glm::vec3 objectColor, glm::vec3 lightColor, float attenuation, float ambientCoefficient, float coneAngle, glm::vec3 coneDirection)
{
	GLuint shaderProgram = Shader::createShader(shaderType);
	auto modelPrefabs = ModelPrefabs::getInstance();
	Model lightModel = modelPrefabs->getModel(modelName);

	return std::make_shared<SpotLight>(position, lightModel, objectColor, shaderProgram, shaderType, lightColor, attenuation, ambientCoefficient, coneAngle, coneDirection);
}
// Spot Light without model
std::shared_ptr<Light> ObjectFactory::createSpotLight(glm::vec3 position, ShaderType shaderType, glm::vec3 lightColor, float attenuation, float ambientCoefficient, float coneAngle, glm::vec3 coneDirection)
{
	GLuint shaderProgram = Shader::createShader(shaderType);

	return std::make_shared<SpotLight>(position, shaderProgram, shaderType, lightColor, attenuation, ambientCoefficient, coneAngle, coneDirection);
}

// Directional Light with model
std::shared_ptr<Light> ObjectFactory::createDirectionalLight(std::string modelName, ShaderType shaderType, glm::vec3 position, glm::vec3 objectColor, glm::vec3 lightColor, glm::vec3 direction, float ambientCoefficient)
{
	GLuint shaderProgram = Shader::createShader(shaderType);
	auto modelPrefabs = ModelPrefabs::getInstance();
	Model lightModel = modelPrefabs->getModel(modelName);

	return std::make_shared<DirectionalLight>(position, lightModel, objectColor, shaderProgram, shaderType, lightColor, direction, ambientCoefficient);
}
// Directional Light without model
std::shared_ptr<Light> ObjectFactory::createDirectionalLight(ShaderType shaderType, glm::vec3 lightColor, glm::vec3 direction, float ambientCoefficient)
{
	GLuint shaderProgram = Shader::createShader(shaderType);

	return std::make_shared<DirectionalLight>(shaderProgram, shaderType, lightColor, direction, ambientCoefficient);
}



