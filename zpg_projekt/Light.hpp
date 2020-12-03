#pragma once

#include <GL/glew.h>	// must be always included first!
#include <GLFW/glfw3.h>  
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include "Object.hpp"


class Light : public Object
{
public:
	enum LightType {
		POINT_LIGHT,
		SPOT_LIGHT,
		DIRECTION_LIGHT
	};
protected:
	LightType lightType;
public:
	Light(glm::vec3 position, Model model, glm::vec3 modelColor, GLuint shaderID, glm::vec3 lightColor, ShaderType shaderType, std::shared_ptr<Camera> camera);
	//Light(glm::vec3 position);
	Light();
	~Light();

	//int lightType;
	glm::vec4 position;
	glm::vec3 color; //a.k.a. the color of the light
	float attenuation;
	float ambientCoefficient;
	float coneAngle; // new
	glm::vec3 coneDirection; // new


	void render();
	LightType getLightType();
	glm::vec3 lightColor;
	float lightIntensity;
};