#pragma once

#include<string>

//#include "Shader.hpp"
#include "models/2/sphere.hpp"
#include "models/2/suzi_flat.hpp"
#include "models/2/suzi_smooth.hpp"
#include "models/2/plain.hpp"

#include "models/1/bedna.hpp"
#include "models/1/worker.hpp"

//#include "Shader.hpp"
#include <GLFW/glfw3.h>


class Model
{
public:
	Model();
	Model(std::string modelName, GLuint vertCount = 0);
	//GLuint getVAO();
	void render();
	void bindVAO();
	//Shader shader;
private:
	GLuint VAO, VBO;
	unsigned int vertexCount;
	
};

