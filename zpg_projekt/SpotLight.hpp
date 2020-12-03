#pragma once

//#include <glm/vec3.hpp>
#include "PointLight.hpp"

class SpotLight : public PointLight
{
public:
	SpotLight(glm::vec3 position, Model model, glm::vec3 modelColor, GLuint shaderID, glm::vec3 lightColor, ShaderType shaderType, std::shared_ptr<Camera> camera);
	//Spotlight();
};

