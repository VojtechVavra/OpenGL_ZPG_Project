#pragma once

//GLEW - The OpenGL Extension Wrangler Library
#include <GL/glew.h>

#include <string.h> 
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <sstream>


class ShaderLoader
{
public:
	ShaderLoader() = default;
	GLuint loadShader(const char* vertexFile, const char* fragmentFile);
	void deleteShader();

private:
	ShaderLoader(const char* vertexFile, const char* fragmentFile, GLuint* shaderID);
	std::string loadFile(const char* fname);

	void checkVertShaderCompileError(GLuint vertShader);
	void checkFragShaderCompileError(GLuint fragShader);
	void checkProgramShaderCompileError(GLuint programID);

private:
	GLuint programID;
};
