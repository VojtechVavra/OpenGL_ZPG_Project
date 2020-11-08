#include <iostream>

#include <glm/vec3.hpp>                 // glm::vec3
#include <glm/vec4.hpp>                 // glm::vec4
#include <glm/mat4x4.hpp>               // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>         // glm::value_ptr
#include "Object.hpp"
#include "Camera.hpp"


GLuint Object::objectCount = 0;

Object::Object(glm::vec3 position)
{
    this->m_matrix = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    Translate(position);

    std::cout << "Object type 1 - camera only" << std::endl;
}

Object::Object(glm::vec3 position, Model model, glm::vec3 color, GLuint shaderProgram, ShaderType shaderType, std::shared_ptr<Camera> camera)
{
    this->m_matrix = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->color = color;
    this->model = model;
    this->shaderProgram = shaderProgram;
    this->shaderType = shaderType;
    this->camera = camera;
    
    this->objID = ++Object::objectCount;

    Translate(position);

    std::cout << "Object type 2 - other model objects" << std::endl;
}

/*Object::Object(glm::vec3 position, Model model, GLuint shaderProgram, ShaderType shaderType)
{
    this->m_matrix = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->color = glm::vec3(1.0f, 1.0f, 1.0f);
    this->model = model;
    this->shaderProgram = shaderProgram;
    this->shaderType = shaderType;
    Translate(position);

    std::cout << "Object 3" << std::endl;
}*/

Object::Object()
{

}

Object::~Object()
{

}

glm::vec3 Object::getPosition() const
{
    return this->position;
}

glm::mat4 Object::getMatrix() const
{
    return this->m_matrix;
}

Model Object::getModel() const
{
    return this->model;
}

GLuint Object::getShader() const
{
    return this->shaderProgram;
}

void Object::useShader()
{
    glUseProgram(shaderProgram);
}

void Object::setPosition(glm::vec3 newPosition)
{
    this->position = newPosition;
    m_matrix = glm::translate(this->getMatrix(), newPosition);
}



void Object::Translate(glm::mat4 mat4x4, glm::vec3 shift)
{
    this->m_matrix = glm::translate(mat4x4, shift);
}

void Object::Translate(glm::vec3 shift)
{
    this->position += shift;
    this->m_matrix = glm::translate(m_matrix, shift);
}


//Note that to convert from degrees to radians, use glm::radians(degrees)
void Object::Rotate(glm::mat4 mat4x4, float rotxangle, glm::vec3 axis)
{
    this->m_matrix = glm::rotate(mat4x4, rotxangle, axis);
}

void Object::Rotate(float rotxangle, glm::vec3 axis)
{
    this->m_matrix = glm::rotate(m_matrix, rotxangle, axis);
}


void Object::Scale(glm::mat4 mat4x4, glm::vec3 scale)
{
    this->m_matrix = glm::scale(mat4x4, scale);
}

void Object::Scale(glm::vec3 scale)
{
    this->m_matrix = glm::scale(m_matrix, scale);
}

glm::vec3 Object::getColor()
{
    return this->color;
}

ShaderType Object::getShaderType()
{
    return this->shaderType;
}

GLuint Object::getID()
{
    return this->objID;
}

void Object::update(std::string change)
{
    //this->use();
    Shader::use(this->getShader());
    if (change == "camera") {
        // nepridavat vypisy/printy sekala by se scena
        if (shaderType == ShaderType::AMBIENT)
        {
            //sendUniform("modelMatrix", scene->object[i].getMatrix());
            Shader::sendUniform(shaderProgram, "viewMatrix", camera->getCamera());
        }
        else if (shaderType == ShaderType::DIFFUSE)
        {
            //sendUniform("modelMatrix", scene->object[i].getMatrix());
            Shader::sendUniform(shaderProgram, "viewMatrix", camera->getCamera());
        }
        else if (shaderType == ShaderType::SPECULAR)
        {
            //sendUniform("modelMatrix", scene->object[i].getMatrix());
            Shader::sendUniform(shaderProgram, "viewMatrix", camera->getCamera());
            Shader::sendUniform(shaderProgram, "viewPos", camera->getPosition());
        }
        else if (shaderType == ShaderType::PHONG)
        {
            //sendUniform("modelMatrix", scene->object[i].getMatrix());
            Shader::sendUniform(shaderProgram, "viewMatrix", camera->getCamera());
            Shader::sendUniform(shaderProgram, "viewPos", camera->getPosition());
        }
        else if (shaderType == ShaderType::BLINN)
        {
            //Shader::sendUniform(shaderProgram, "modelMatrix", scene->object[i].getMatrix());
            //Shader::sendUniform(shaderProgram, "viewMatrix", m_camera->getCamera());
            //Shader::sendUniform(shaderProgram, "viewPos", m_camera->getPosition());
        }
        //std::cout << "Camera has changed" << std::endl;
    }
    else if (change == "projection") {
        // zoom / fov changed
        Shader::sendUniform(shaderProgram, "projectionMatrix", camera->getProjectionMatrix());
        //std::cout << "Projection has changed" << std::endl;
    }
}