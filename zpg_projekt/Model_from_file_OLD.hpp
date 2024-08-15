#pragma once

#include<string>
#include<vector>

#include<assimp/Importer.hpp>			// C++ importerinterface
#include<assimp/scene.h>				// aiSceneoutputdata structure
#include<assimp/postprocess.h>			// Post processingflags

#include <glm/vec3.hpp>                 // glm::vec3
#include <glm/vec4.hpp>                 // glm::vec4
#include <glm/mat4x4.hpp>               // glm::mat4

#include "models/2/sphere.hpp"
#include "models/2/suzi_flat.hpp"
#include "models/2/suzi_smooth.hpp"
#include "models/2/plain.hpp"
#include "models/2/cube.hpp"

#include "models/1/bedna.hpp"
#include "models/1/worker.hpp"

#include <GLFW/glfw3.h>

// https://www.cs.utexas.edu/~fussell/courses/cs384g-spring2017/lectures/Lecture9-Zbuffer_pipeline.pdf

class Model_from_file_OLD
{
public:
	Model_from_file_OLD();
	Model_from_file_OLD(std::string modelName, GLuint vertCount = 0);
	Model_from_file_OLD(std::string modelName, bool _isTextured);
	//GLuint getVAO();
	void render();
	void bindVAO();
	//Shader shader;
private:
	GLuint VAO, VBO;
	unsigned int vertexCount;

	static const std::vector<float> cubemapPoints;
};
