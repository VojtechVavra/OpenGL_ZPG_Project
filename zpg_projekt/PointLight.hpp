#pragma once

#include "Light.hpp"

class PointLight : public Light
{
public:
    PointLight(glm::vec3 position, Model model, glm::vec3 modelColor, GLuint shaderID, glm::vec3 lightColor, ShaderType shaderType, std::shared_ptr<Camera> camera);
};

