#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>   // glm::mat4
#include <glm/gtc/type_ptr.hpp>
#include "ShaderLoader.hpp"
#include "Shader.hpp"
#include "Camera.hpp"


//https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/7.4.camera_class/camera_class.cpp

Shader::Shader() {
    //m_camera = nullptr;
    //createShader();
}
Shader::Shader(int i) {
    createShader();
    glUseProgram(shaderProgram);
}

Shader::Shader(Camera* camera)
{
    m_camera = camera;
    createShader();
    //glUseProgram(shaderProgram);
}

GLuint Shader::createShader()
{
    ShaderLoader loader;
    this->shaderProgram = loader.loadShader("./vertex.glsl", "./fragment.glsl");
    //this->shaderProgram = loader.loadShader("./vertex2.glsl", "./fragment2.glsl");
    return this->shaderProgram;
}

GLuint Shader::getShader()
{
    return this->shaderProgram;
}

/*GLuint Shader::CreateAndCompile() {
    // 4 Create and compile shaders

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader, NULL);
    glCompileShader(vertexShader);
    VertShaderCompileError(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader, NULL);
    glCompileShader(fragmentShader);
    FragShaderCompileError(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, fragmentShader);
    glAttachShader(shaderProgram, vertexShader);
    glLinkProgram(shaderProgram);

    this->shaderProgram = shaderProgram;
    return shaderProgram;
}*/

/*void Shader::VertShaderCompileError(GLuint vertShader) {
    GLint result;
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);

    if (result == 0) {
        std::cout << "Vertex shader compilation failed!\n";
        GLint logLen;

        glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0) {
            char* log = (char*)malloc(logLen);
            GLsizei writter;
            glGetShaderInfoLog(vertShader, logLen, &writter, log);
            std::cout << "Vertex Shader log:\n" << log << std::endl;
            free(log);
        }
    }
}*/

/*void Shader::FragShaderCompileError(GLuint fragShader) {
    GLint result;
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);

    if (result == 0) {
        std::cout << "Fragment shader compilation failed!\n";
        fflush(stdout);

        GLint logLen;
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLen);

        if (logLen > 0) {
            char* log = (char*)malloc(logLen);
            GLsizei writter;
            glGetShaderInfoLog(fragShader, logLen, &writter, log);
            std::cout << "Fragment Shader log:\n" << log << std::endl;
            free(log);
        }
    }
}*/

void Shader::sendUniform(const GLchar* name, GLfloat value)
{
    GLint uniformID = glGetUniformLocation(this->shaderProgram, name);  // check for -1, nepouzita uniformni promenna (otestovano)
    if (uniformID >= 0) {
        glUniform1f(uniformID, value);
    }
    else {
        // in shader doesn't exist uniform variable 
        std::cout << "promenna neexistuje(0) " << name << std::endl;
    }
    
}

void Shader::sendUniform(const GLchar* name, glm::vec3 pos) {
    GLint uniformID = glGetUniformLocation(this->shaderProgram, name);  // check for -1, nepouzita uniformni promenna (otestovano)
    if (uniformID >= 0) {
        //glUniformMatrix4fv(uniformID, 1, GL_FALSE,  glm::value_ptr(data));
        //glUniform3f(uniformID, data.x, data.y, data.z);
        glProgramUniform3f(this->shaderProgram, uniformID, pos.x, pos.y, pos.z); // aktualizace dat
    }
    else {
        // in shader doesn't exist uniform variable 
        // zastavit program zde
        std::cout << "promenna neexistuje(1) " << name << std::endl;
    }
}

void Shader::sendUniform(const GLchar* name, glm::vec4 data) {
    GLint uniformID = glGetUniformLocation(this->shaderProgram, name);  // check for -1, nepouzita uniformni promenna (otestovano)
    if (uniformID >= 0) {
        //glUniformMatrix4fv(uniformID, 1, GL_FALSE,  glm::value_ptr(data));
        glUniform4f(uniformID, data.x, data.y, data.z, data.w);
    }
    else {
        // in shader doesn't exist uniform variable 
        // zastavit program zde
        std::cout << "promenna neexistuje(2) " << name << std::endl;
    }
}

void Shader::sendUniform(const GLchar* name, glm::mat4 data) {
    GLint uniformID = glGetUniformLocation(this->shaderProgram, name);
    if (uniformID >= 0) {
        glUniformMatrix4fv(uniformID, 1, GL_FALSE, glm::value_ptr(data));
    }
    else {
        // in shader doesn't exist uniform variable 
        std::cout << "promenna neexistuje(3) " << name << std::endl;
    }
}

/*
void Shader::Update()
{
    //this->m_camera->updateCameraVectors();

    GLint uniformID = glGetUniformLocation(this->shaderProgram, "viewMatrix");
    //glm::mat4 MVP = transform.GetMVP(camera);

    glm::mat4 VP = m_camera->getCamera();
    //glm::mat4 VP = m_camera->GetViewMatrix();

    glUniformMatrix4fv(uniformID, 1, GL_FALSE, glm::value_ptr(VP));
    //glUniformMatrix4fv(uniformID, 1, GL_FALSE, &MVP[0][0]);

    sendUniform("projectionMatrix", m_camera->getProjectionMatrix());
}*/

void Shader::use()
{
    glUseProgram(shaderProgram);
}

void Shader::createMe()
{
    /* This is a handle to the shader program */
    //GLuint shaderprogram;

    /* Allocate and assign a Vertex Array Object to our handle */
    //glGenVertexArrays(1, &vao);

    /* Bind our Vertex Array Object as the current used object */
    //glBindVertexArray(vao);

    /* Allocate and assign two Vertex Buffer Objects to our handle */
    //glGenBuffers(2, vbo);

    /* Bind our first VBO as being the active buffer and storing vertex attributes (coordinates) */
    //glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

    /* Copy the vertex data from diamond to our buffer */
    /* 8 * sizeof(GLfloat) is the size of the diamond array, since it contains 8 GLfloat values */
    //glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), diamond, GL_STATIC_DRAW);

    /* Specify that our coordinate data is going into attribute index 0, and contains two floats per vertex */
    //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    /* Enable attribute index 0 as being used */
    //glEnableVertexAttribArray(0);

    /* Bind our second VBO as being the active buffer and storing vertex attributes (colors) */
   // glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

    /* Copy the color data from colors to our buffer */
    /* 12 * sizeof(GLfloat) is the size of the colors array, since it contains 12 GLfloat values */
   // glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

    /* Specify that our color data is going into attribute index 1, and contains three floats per vertex */
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /* Enable attribute index 1 as being used */
    //glEnableVertexAttribArray(1);

}