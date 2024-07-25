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

// Constructor for camera
Object::Object(glm::vec3 position)
{   // pouzivam jen pro kameru
    this->m_matrix = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotation = glm::vec3(0.0f);
    this->scale = glm::vec3(0.0f);
    Translate(position);

    this->color = glm::vec3(1.0f, 1.0f, 1.0f);
    std::cout << "Object type 1 - camera only" << std::endl;
}

// Constructor for lights without model
Object::Object(glm::vec3 position, GLuint shaderProgram, ShaderType shaderType)
{
    this->m_matrix = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotation = glm::vec3(0.0f);
    this->scale = glm::vec3(0.0f);
    //this->color = color;
    this->color = glm::vec3(1.0f, 1.0f, 1.0f);
    //this->model = nullptr;
    this->shaderProgram = shaderProgram;
    this->shaderType = shaderType;
    //this->camera = camera;
    //this->hasTexture = false;
    //this->objID = Object::objectCount += 1;

    Translate(position);

    std::cout << "Object type 2 - light without model" << std::endl;
}

Object::Object(glm::vec3 position, Model model, glm::vec3 color, GLuint shaderProgram, ShaderType shaderType)
{
    this->m_matrix = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotation = glm::vec3(0.0f);
    this->scale = glm::vec3(0.0f);
    this->color = color;
    this->model = model;
    this->shaderProgram = shaderProgram;
    this->shaderType = shaderType;
    //this->camera = camera;
    //this->hasTexture = false;

    this->objID = Object::objectCount += 1;

    Translate(position);

    std::cout << "Object type 3 - other model objects" << std::endl;
}

Object::Object(glm::vec3 position, Model model, std::shared_ptr<Texture> texture, GLuint shaderProgram, ShaderType shaderType)
{
    this->m_matrix = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotation = glm::vec3(0.0f);
    this->scale = glm::vec3(0.0f);
    this->texture = texture;
    this->model = model;
    this->shaderProgram = shaderProgram;
    this->shaderType = shaderType;
    //this->hasTexture = false;
    //this->hasTexture = true;

    this->objID = Object::objectCount += 1;

    Translate(position);

    std::cout << "Object type 4 - texture model object" << std::endl;
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
    this->rotation = axis * rotxangle;
}

void Object::Rotate(float rotxangle, glm::vec3 axis)
{
    this->m_matrix = glm::rotate(m_matrix, rotxangle, axis);
    this->rotation += axis * rotxangle;
}


void Object::Scale(glm::mat4 mat4x4, glm::vec3 scale)
{
    this->m_matrix = glm::scale(mat4x4, scale);
    this->scale = scale;
}

void Object::Scale(glm::vec3 scale)
{
    this->m_matrix = glm::scale(m_matrix, scale);
    this->scale += scale;
}


bool Object::hasTexture()
{
    return texture != nullptr;
}

void Object::loadTexture()
{
    if(texture != nullptr)
        texture->Load();
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

glm::vec3 Object::getRotate() const
{
    return this->rotation;
}

glm::vec3 Object::getScale() const
{
    return this->scale;
}

/*void Object::update(Camera* camera, camChange change)
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

            // flashlight(as Spotlight) on Camera 
            if (camera->isFlashLightOn()) {

                Shader::sendUniform(shaderProgram, "flashLight.position", camera->getPosition());
                Shader::sendUniform(shaderProgram, "flashLight.direction", camera->target);
            }
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

            //Shader::sendUniform(shaderProgram, "fragmentColor", glm::vec3(0.f, 0.f, 1.0f));

            // SpotLight
            //Shader::sendUniform(shaderProgram, "spotLight[0].position", camera->getPosition());
            //Shader::sendUniform(shaderProgram, "spotLight[0].direction", camera->target);

            // flashlight(as Spotlight) on Camera 
            if (camera->isFlashLightOn()) {
                
                Shader::sendUniform(shaderProgram, "flashLight.position", camera->getPosition());
                Shader::sendUniform(shaderProgram, "flashLight.direction", camera->target);
            }
            
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
    else if (change == FLASHLIGHT) {
        if (shaderType == ShaderType::DIFFUSE)
        {
            Shader::sendUniform(shaderProgram, "flashLight.isActive", camera->isFlashLightOn() ? 1 : 0);
            if (camera->isFlashLightOn()) {
                Shader::sendUniform(shaderProgram, "flashLight.position", camera->getPosition());
                Shader::sendUniform(shaderProgram, "flashLight.direction", camera->target);
            }
        }
        else if (shaderType == ShaderType::PHONG)
        {
            Shader::sendUniform(shaderProgram, "flashLight.isActive", camera->isFlashLightOn() ? 1 : 0);
            if (camera->isFlashLightOn()) {
                Shader::sendUniform(shaderProgram, "flashLight.position", camera->getPosition());
                Shader::sendUniform(shaderProgram, "flashLight.direction", camera->target);
            }
        }
    }
}*/


void Object::render()
{
    this->getModel().bindVAO();
    
    this->useShader();  // use shaderProgram
    //glUseProgram(Shader::getShader(getShaderType()));
    
    //Shader::sendUniform(Shader::getShader(getShaderType()), "modelMatrix", getMatrix());
    Shader::sendUniform(getShader(), "modelMatrix", getMatrix());

    if (getShaderType() == ShaderType::PHONG /*|| (getShaderType() == ShaderType::DIFFUSE)*/)
    {
        if (hasTexture()) {
            loadTexture();
            Shader::sendUniform(getShader(), "hasTexture", 1);

            Shader::sendUniform(getShader(), "textureUnitID", 0);
            //GLint uniformID = glGetUniformLocation(getShader(), "textureUnitID");
            //glUniform1i(uniformID, 0);
        }
        else {
            Shader::sendUniform(getShader(), "hasTexture", 0);
            Shader::sendUniform(getShader(), "fragmentColor", getColor());
        }
    }
    
    
    //Shader::sendUniform(getShader(), "fragmentColor", glm::vec3(1.f, 0.0f, 0.0f));

    this->getModel().render();		// glDrawArrays()
}


void Object::renderSkybox(std::shared_ptr<Camera> camera)
{
    this->getModel().bindVAO();

    this->useShader();  // use shaderProgram
    //glUseProgram(Shader::getShader(getShaderType()));

    //Shader::sendUniform(Shader::getShader(getShaderType()), "modelMatrix", getMatrix());
    Shader::sendUniform(getShader(), "modelMatrix", getMatrix());
    Shader::sendUniform(getShader(), "projectionMatrix", camera->getProjectionMatrix());
    Shader::sendUniform(getShader(), "skybox", 0);

    texture->LoadCubemap();

    this->getModel().render();		// glDrawArrays()
    glBindVertexArray(0);
}

void Object::renderSkybox2(std::shared_ptr<Camera> camera)
{
    glDepthMask(GL_FALSE);
    this->useShader();  // use shaderProgram -  glUseProgram(shaderProgram);
    //cubeMap->bind(shader);
    glStencilFunc(GL_ALWAYS, -1, 0xFF);
    this->getModel().bindVAO();  //glBindVertexArray(vao);
    //glDrawArrays(GL_TRIANGLES, 0, points.size());
    this->getModel().render();		// glDrawArrays()
    glDepthMask(GL_TRUE);
}