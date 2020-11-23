#include <iostream>

#include <glm/vec3.hpp>                 // glm::vec3
#include <glm/vec4.hpp>                 // glm::vec4
#include <glm/mat4x4.hpp>               // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>         // glm::value_ptr

#include "Object.hpp"
#include "Camera.hpp"
#include "Subject.hpp"


GLuint Object::objectCount = 0;

Object::Object(glm::vec3 position)
{   // pouzivam jen pro kameru
    this->m_matrix = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    Translate(position);

    this->color = glm::vec3(1.0f, 1.0f, 1.0f);
    std::cout << "Object type 1 - camera only" << std::endl;
}

Object::Object(glm::vec3 position, Model model, glm::vec3 color, GLuint shaderProgram, ShaderType shaderType)
{
    this->m_matrix = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->color = color;
    this->model = model;
    this->shaderProgram = shaderProgram;
    this->shaderType = shaderType;
    //this->camera = camera;
    
    this->objID = Object::objectCount += 1;

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
    this->color = glm::vec3(1.0f, 1.0f, 1.0f);
}

/*Object::~Object()
{

}*/

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

void Object::setMatrix(glm::mat4 newMatrix) {
    this->m_matrix = newMatrix;
}

void Object::setPosition(glm::vec3 newPosition)
{
    this->position = newPosition;
    m_matrix = glm::translate(glm::mat4(1.0f), newPosition);
    //m_matrix = glm::translate(this->getMatrix(), newPosition);
}
void Object::setPositionWithoutTranslate(glm::vec3 newPosition)
{
    this->position = newPosition;
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

void Object::LookAt(glm::vec3 targetPosition, float anglex, float angley)
{
    // move camera a distance r away from the center
    //glTranslatef(0.f, 0.f, 2.0f);
    Translate(targetPosition);

    Rotate(angley, glm::vec3(0.0f, 1.0f, 0.0f));
    Rotate(anglex, glm::vec3(1.0f, 0.0f, 0.0f));
    // rotate 


    // move to center of circle  
    Translate(-targetPosition);
    //glTranslatef(-cx, -cy, -cz)

    //glm::vec3 up(1.0f, 1.0f, 1.0f);
   // this->m_matrix = glm::lookAt(position, -(position + targetPosition), up);

    //Rotate(x, glm::vec3(1.0f, 0.0f, 0.0f));
    //Rotate(y, glm::vec3(0.0f, 1.0f, 0.0f));

    /*glm::vec3 up(1.0f, 1.0f, 1.0f);
    glm::mat4 newLook = glm::lookAt(this->position, target, up);
    this->m_matrix *= newLook;*/

    //Double_t RadToDeg()
    //{
    //    return 180.0 / Pi();
    //}

    /*double radToDeg = 180.0 / 3.141592653589;
    if ((targetPosition - position) == glm::vec3(0, 0, 0)) return;
    glm::vec3 direction = glm::normalize(targetPosition - position);
    float m_rotation_x = asinf(-direction.y) * radToDeg;
    float m_rotation_y = -atan2f(-direction.x, -direction.z) * radToDeg;

    Rotate(m_rotation_x, glm::vec3(1.0f, 0.0f, 0.0f));
    Rotate(m_rotation_y, glm::vec3(0.0f, 1.0f, 0.0f));*/

    //NormalizeAngles();
}


glm::vec3 Object::getColor()
{
    return this->color;
}

void Object::setNewColor(glm::vec3 newColor)
{
    this->lastColor = this->getColor();
    this->color = newColor;
}

void Object::setLastColor()
{
    this->color = this->lastColor;
}

ShaderType Object::getShaderType()
{
    return this->shaderType;
}

GLuint Object::getID()
{
    return this->objID;
}

/*void Object::update(std::string change)
{
    //this->use();
    Shader::use(this->getShader());
    if (change == "camera") {
        // nepridavat vypisy/printy sekala by se scena
        if (this->shaderType == ShaderType::AMBIENT)
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
}*/

void Object::update(Camera* camera, camChange change)
{
    Shader::use(this->getShader());
    if (change == camChange::MOVE_ROTATE) {
        // nepridavat vypisy/printy sekala by se scena
        if (this->shaderType == ShaderType::AMBIENT)
        {
            Shader::sendUniform(shaderProgram, "modelMatrix", getMatrix());

            Shader::sendUniform(shaderProgram, "viewMatrix", camera->getCamera());
        }
        else if (shaderType == ShaderType::DIFFUSE)
        {
            Shader::sendUniform(shaderProgram, "modelMatrix", getMatrix());
            //sendUniform("modelMatrix", scene->object[i].getMatrix());
            Shader::sendUniform(shaderProgram, "viewMatrix", camera->getCamera());
        }
        else if (shaderType == ShaderType::SPECULAR)
        {
            Shader::sendUniform(shaderProgram, "modelMatrix", getMatrix());
            //sendUniform("modelMatrix", scene->object[i].getMatrix());
            Shader::sendUniform(shaderProgram, "viewMatrix", camera->getCamera());
            Shader::sendUniform(shaderProgram, "viewPos", camera->getPosition());
        }
        else if (shaderType == ShaderType::PHONG)
        {
            Shader::sendUniform(shaderProgram, "modelMatrix", getMatrix());
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
    else if (change == camChange::PROJECTION) {
        // zoom / fov changed
        Shader::sendUniform(shaderProgram, "projectionMatrix", camera->getProjectionMatrix());
        //std::cout << "Projection has changed" << std::endl;
    }
}