#pragma once

#include<string>
#include "Shader.hpp"
#include "sphere.hpp"
#include "suzi_smooth.hpp"
//#include "Shader.hpp"
#include <GLFW/glfw3.h>


class Model
{
public:
	Model();
	Model(std::string modelName/*, Shader shader*/, GLuint vertCount = 0);
	GLuint getVAO();
	void render();
	//Shader shader;
private:
	GLuint VAO, VBO;
	unsigned int vertexCount;
	
};

