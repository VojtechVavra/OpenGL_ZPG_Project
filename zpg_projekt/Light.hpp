#pragma once

#include <GLFW/glfw3.h>  
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include "Object.hpp"


class Light : public Object
{
private:
	GLuint VAO;
public:
	Light(glm::vec3 position);
	~Light();
	void render();
};