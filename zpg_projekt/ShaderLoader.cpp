#include "ShaderLoader.hpp"

std::string ShaderLoader::loadFile(const char* fname)
{
	std::ifstream file(fname);
	if (!file.is_open())
	{
		std::cout << "Unable to open file " << fname << std::endl;
		//exit(1);
	}
	std::stringstream fileData;
	fileData << file.rdbuf();
	file.close();
	return fileData.str();
}

GLuint ShaderLoader::loadShader(const char* vertexFile, const char* fragmentFile) {
	printf("Shader::loadShader %s %s \n", vertexFile, fragmentFile);
	std::string vertexShaderString = loadFile(vertexFile);
	std::string fragmentShaderString = loadFile(fragmentFile);
	int vlen = vertexShaderString.length();
	int flen = fragmentShaderString.length();

	if (vertexShaderString.empty()) printf("Nulový Vertex Shader\n");
	if (fragmentShaderString.empty()) printf("Nulový Fragment Shader\n");

	const char* vertexShaderCStr = vertexShaderString.c_str();
	const char* fragmentShaderCStr = fragmentShaderString.c_str();

	vertexID = glCreateShader(GL_VERTEX_SHADER);    //Vertex Shader
	glShaderSource(vertexID, 1, (const GLchar**)&vertexShaderCStr, &vlen);
	glCompileShader(vertexID);

	// Check for vertex shader compilation errors
	checkVertShaderCompileError(vertexID);

	fragmentID = glCreateShader(GL_FRAGMENT_SHADER); //Fragment Shader
	glShaderSource(fragmentID, 1, (const GLchar**)&fragmentShaderCStr, &flen);
	glCompileShader(fragmentID);

	// Check for fragment shader compilation errors
	checkFragShaderCompileError(fragmentID);
	
	programID = glCreateProgram();
	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);
	glLinkProgram(programID);

	// Check for shader program compilation errors
	checkProgramShaderCompileError(programID);


	return programID;
}

void ShaderLoader::deleteShader() {
	printf("ShaderLoader::deleteShader()\n");
	glDetachShader(programID, vertexID);
	glDetachShader(programID, fragmentID);
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
	glUseProgram(0);
	glDeleteProgram(this->programID);
}

ShaderLoader::ShaderLoader(const char* vertexFile, const char* fragmentFile, GLuint* shaderID) {

	*shaderID = loadShader(vertexFile, fragmentFile);
}


void ShaderLoader::checkVertShaderCompileError(GLuint vertShader) {
	GLint status;
	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(vertexID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* strInfoLog = new GLchar[infoLogLength];
		glGetShaderInfoLog(vertexID, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Compile failure in Vertex shader:\n%s\n", strInfoLog);
		delete[] strInfoLog;
	}
}

void ShaderLoader::checkFragShaderCompileError(GLuint fragShader) {
	GLint status;
	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint logLen;
		glGetShaderiv(fragmentID, GL_INFO_LOG_LENGTH, &logLen);

		if (logLen > 0) {
			GLchar* log = (GLchar*)malloc(logLen);
			GLsizei writter;
			glGetShaderInfoLog(fragmentID, logLen, &writter, log);
			fprintf(stderr, "Compile failure in Fragment shader:\n%s\n", log);
			free(log);
		}
	}
}

void ShaderLoader::checkProgramShaderCompileError(GLuint programID)
{
	GLint status;
	glGetProgramiv(programID, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint logLen;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLen);

		GLchar* strInfoLog = new GLchar[logLen + 1];
		glGetProgramInfoLog(programID, logLen, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
		exit(EXIT_FAILURE);
	}
}

ShaderLoader::ShaderLoader()
{
}


ShaderLoader::~ShaderLoader()
{
}
