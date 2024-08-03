#pragma once

#include <GL/glew.h>	// must be always included first!
#include <GLFW/glfw3.h>

//#include "Shader.hpp"
#include "IObserver.hpp"

class Camera; // Forward declaration of Camera
enum class ShaderType;
//#include "Camera.hpp"


class ShaderProgram  : public IObserver
{
public:
	ShaderProgram(ShaderType shaderType, GLuint shaderProgram);
	ShaderProgram();

	void update(Camera* camera, enum class camChange cameraChange) override;

	ShaderType shaderType;
	GLuint shaderProgram;
};
