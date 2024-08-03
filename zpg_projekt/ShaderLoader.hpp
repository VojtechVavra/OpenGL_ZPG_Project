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
private:
	GLuint programID;
	std::string loadFile(const char* fname);

public:
	GLuint loadShader(const char* vertexFile, const char* fragmentFile);
	void deleteShader();
	ShaderLoader() = default;
	ShaderLoader(const char* vertexFile, const char* fragmentFile, GLuint* shaderID);
	void checkVertShaderCompileError(GLuint vertShader);
	void checkFragShaderCompileError(GLuint fragShader);
	void checkProgramShaderCompileError(GLuint programID);
	~ShaderLoader();
};
