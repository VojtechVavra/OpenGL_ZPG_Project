#pragma once

#include "Light.hpp"

class PointLight : public Light
{
public:
    PointLight(glm::vec3 position, /*Model model,*/ glm::vec3 modelColor, GLuint shaderProgram, ShaderType shaderType, glm::vec3 lightColor, float attenuation, float ambientCoefficient);
    PointLight(glm::vec3 position, GLuint shaderProgram, ShaderType shaderType, glm::vec3 lightColor, float attenuation, float ambientCoefficient);
private:
    float attenuation;
    //float ambientCoefficient;
};

