//Include GLM  
#include <glm/vec3.hpp>                 // glm::vec3
#include <glm/vec4.hpp>                 // glm::vec4
#include <glm/mat4x4.hpp>               // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>         // glm::value_ptr
#include "Object.hpp"


Object::Object(glm::vec3 position)
{
    this->m_matrix = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    Translate(position);
}

Object::Object(glm::vec3 position, Model model, GLuint shaderID)
{
    this->m_matrix = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->model = model;
    this->shaderID = shaderID;
    Translate(position);
}

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
    return this->shaderID;
}

void Object::useShader()
{
    glUseProgram(shaderID);
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