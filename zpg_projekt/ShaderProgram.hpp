#pragma once

#include <GL/glew.h>	// must be always included first!
#include <GLFW/glfw3.h>

//#include "Shader.hpp"
#include "Observer.hpp"
#include "Camera.hpp"


class ShaderProgram  : public /*virtual*/ Observer
{
public:
	ShaderProgram(ShaderType shaderType, GLuint shaderProgram);
	ShaderProgram();

	void update(Camera* camera, enum class camChange cameraChange) override;

	ShaderType shaderType;
	GLuint shaderProgram;
};

