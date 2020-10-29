#pragma once

#include <GLFW/glfw3.h>  
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include "Object.hpp"


class Light : public Object
{
private:
	//GLuint VAO;
public:
	Light(glm::vec3 position, Model model, glm::vec3 modelColor, GLuint shaderID, glm::vec3 lightColor, ShaderType shaderType, std::shared_ptr<Camera> camera);
	//Light(glm::vec3 position);
	Light();
	~Light();
	void render();
	glm::vec3 lightColor;
	float lightIntensity;
};