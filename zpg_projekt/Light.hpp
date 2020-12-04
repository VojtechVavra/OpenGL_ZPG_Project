#pragma once

#include <GL/glew.h>	// must be always included first!
#include <GLFW/glfw3.h>  
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include "Object.hpp"


class Light : public Object
{
public:
	enum class LightType {
		POINT_LIGHT,
		SPOT_LIGHT,
		DIRECTION_LIGHT
	};
protected:
	LightType lightType;
public:
	Light(glm::vec3 position, Model model, glm::vec3 modelColor, GLuint shaderProgram, ShaderType shaderType, glm::vec3 lightColor, float ambientCoefficient);
	Light(glm::vec3 position, GLuint shaderProgram, ShaderType shaderType, glm::vec3 lightColor, float ambientCoefficient);

	//Light(glm::vec3 position);
	//Light();
	//~Light();


	//int lightType;
	//glm::vec4 position;
	//glm::vec3 color; //a.k.a. the color of the light



	// point light
	//float attenuation;
	//float ambientCoefficient;

	// spot light
	//float coneAngle; // new
	//glm::vec3 coneDirection; // new


	void render();
	LightType getLightType();
	glm::vec3 lightColor;
	float ambientCoefficient;
	//float lightIntensity;
};