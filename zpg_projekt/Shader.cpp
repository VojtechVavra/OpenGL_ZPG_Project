#include <iostream>
#include <cstdio>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>   // glm::mat4
#include <glm/gtc/type_ptr.hpp>

#include "ShaderLoader.hpp"
#include "Shader.hpp"
#include "Camera.hpp"


//https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/7.4.camera_class/camera_class.cpp

//Shader::Shader() {
//}

/*Shader::Shader(int i) {
    createShader(ShaderType::PHONG);
    //createShader();
    glUseProgram(shaderProgram);
}*/

/*Shader::Shader(Camera* camera, ShaderType shaderType)
{
    //m_camera = camera;
    type = shaderType;
    //createShader();
    createShader(shaderType);
}*/

/*GLuint Shader::createShader(ShaderType fragmentShaderType)
{
    ShaderLoader loader;
    const std::string fragmentShaderName = fragShaderPath[fragmentShaderType];

    //this->shaderProgram = loader.loadShader("./vertex.glsl", "./phongFragment.glsl");
    this->shaderProgram = loader.loadShader("./shaders/vertex.glsl", fragmentShaderName.c_str());

    return this->shaderProgram;
}*/

GLuint Shader::createShader(ShaderType fragmentShaderType)
{
    ShaderLoader loader;
    const std::string fragmentShaderName = fragShaderPath[fragmentShaderType];

    return loader.loadShader("./shaders/vertex.glsl", fragmentShaderName.c_str());
}

/*GLuint Shader::createShader()
{
    ShaderLoader loader;

    //this->shaderProgram = loader.loadShader("./vertex.glsl", "./phongFragment.glsl");
    this->shaderProgram = loader.loadShader("./vertex.glsl", "./phongFragment.glsl");
    //this->shaderProgram = loader.loadShader("./vertex2.glsl", "./fragment2.glsl");
    return this->shaderProgram;
}*/

GLuint Shader::getShader()
{
    return this->shaderProgram;
}

ShaderType Shader::getType()
{
    return this->type;
}




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


//
// static functions
//

void Shader::sendUniform(GLuint shaderProgram, const GLchar* name, GLint value)
{
    GLint uniformID = glGetUniformLocation(shaderProgram, name);  // check for -1, nepouzita uniformni promenna (otestovano)
    if (uniformID >= 0) {
        glUniform1i(uniformID, value);
    }
    else {
        // in shader doesn't exist uniform variable 
        std::cout << "promenna neexistuje(static 0) " << name << std::endl;
    }
}

void Shader::sendUniform(GLuint shaderProgram, const GLchar* name, GLfloat value)
{
    GLint uniformID = glGetUniformLocation(shaderProgram, name);  // check for -1, nepouzita uniformni promenna (otestovano)
    if (uniformID >= 0) {
        glUniform1f(uniformID, value);
    }
    else {
        // in shader doesn't exist uniform variable 
        std::cout << "promenna neexistuje(static 0) " << name << std::endl;
    }
}

void Shader::sendUniform(GLuint shaderProgram, const GLchar* name, glm::vec3 pos) {
    GLint uniformID = glGetUniformLocation(shaderProgram, name);  // check for -1, nepouzita uniformni promenna (otestovano)
    if (uniformID >= 0) {
        glProgramUniform3f(shaderProgram, uniformID, pos.x, pos.y, pos.z); // aktualizace dat
    }
    else {
        // in shader doesn't exist uniform variable 
        std::cout << "promenna neexistuje(static 1) " << name << std::endl;
    }
}

void Shader::sendUniform(GLuint shaderProgram, const GLchar* name, glm::vec4 data) {
    GLint uniformID = glGetUniformLocation(shaderProgram, name);  // check for -1, nepouzita uniformni promenna (otestovano)
    if (uniformID >= 0) {
        glUniform4f(uniformID, data.x, data.y, data.z, data.w);
    }
    else {
        // in shader doesn't exist uniform variable 
        std::cout << "promenna neexistuje(static 2) " << name << std::endl;
    }
}

void Shader::sendUniform(GLuint shaderProgram, const GLchar* name, glm::mat4 data) {
    GLint uniformID = glGetUniformLocation(shaderProgram, name);
    if (uniformID >= 0) {
        glUniformMatrix4fv(uniformID, 1, GL_FALSE, glm::value_ptr(data));
    }
    else {
        // in shader doesn't exist uniform variable 
        std::cout << "promenna neexistuje(3) " << name << std::endl;
    }
}
// end static functions


void Shader::use()
{
    glUseProgram(shaderProgram);
}

void Shader::use(GLuint shaderProgram)
{
    glUseProgram(shaderProgram);
}

/*void Shader::update(std::string change)
{

    this->use();
    if (change == "camera") {
        //use();
        // nepridavat vypisy/printy sekala by se scena
        if (getType() == ShaderType::AMBIENT)
        {
            //sendUniform("modelMatrix", scene->object[i].getMatrix());
            sendUniform("viewMatrix", m_camera->getCamera());
        }
        else if (getType() == ShaderType::DIFFUSE)
        {
            //sendUniform("modelMatrix", scene->object[i].getMatrix());
            sendUniform("viewMatrix", m_camera->getCamera());
        }
        else if (getType() == ShaderType::SPECULAR)
        {
            //sendUniform("modelMatrix", scene->object[i].getMatrix());
            sendUniform("viewMatrix", m_camera->getCamera());
            sendUniform("viewPos", m_camera->getPosition());
        }
        else if (getType() == ShaderType::PHONG)
        {
            //sendUniform("modelMatrix", scene->object[i].getMatrix());
            sendUniform("viewMatrix", m_camera->getCamera());
            sendUniform("viewPos", m_camera->getPosition());
        }
        else if (getType() == ShaderType::BLINN)
        {
            //sendUniform("modelMatrix", scene->object[i].getMatrix());
            //sendUniform("viewMatrix", m_camera->getCamera());
            //sendUniform("viewPos", m_camera->getPosition());
        }
        //std::cout << "Camera has changed" << std::endl;
    }
    else if (change == "projection") {
        // zoom / fov changed
        sendUniform("projectionMatrix", m_camera[0].getProjectionMatrix());
        //std::cout << "Projection has changed" << std::endl;
    }
}*/

//void Shader::createMe()
//{
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

//}