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
/*Object::Object(glm::vec3 position, GLuint shaderProgram, ShaderType shaderType)
{
    this->m_matrix = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotation = glm::vec3(0.0f);
    this->scale = glm::vec3(0.0f);
    //this->color = color;
    this->color = glm::vec3(1.0f, 1.0f, 1.0f);

    Translate(position);

    std::cout << "Object type 2 - light without model" << std::endl;
}*/

// Colored 3D model
/*Object::Object(glm::vec3 position, glm::vec3 color, GLuint shaderProgram, ShaderType shaderType)
{
    this->m_matrix = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotation = glm::vec3(0.0f);
    this->scale = glm::vec3(0.0f);
    this->color = color;
    //this->model = model;
    //this->shaderProgram = shaderProgram;
    //this->shaderType = shaderType;
    //this->camera = camera;
    //this->hasTexture = false;

    this->objID = Object::objectCount += 1;

    Translate(position);

    std::cout << "Object type 3 - other model objects" << std::endl;
}*/

// Textured 3D model
/*Object::Object(glm::vec3 position, std::shared_ptr<Texture> texture, GLuint shaderProgram, ShaderType shaderType)
{
    this->m_matrix = glm::mat4(1.0f);
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotation = glm::vec3(0.0f);
    this->scale = glm::vec3(0.0f);
    this->texture = texture;
    //this->model = model;
    //this->shaderProgram = shaderProgram;
    //this->shaderType = shaderType;
    //this->hasTexture = false;
    //this->hasTexture = true;

    this->objID = Object::objectCount += 1;

    Translate(position);

    std::cout << "Object type 4 - texture model object" << std::endl;
}*/


Object::Object()
{
    this->color = glm::vec3(1.0f, 1.0f, 1.0f);
}

#include "models/2/cube.hpp"

// New after refactoring used
Object::Object(const std::string& name, const ShaderType shaderType)
{
    m_shader = std::make_shared<Shader>(shaderType);
    MeshLoader a(name.c_str(), m_mesh);
    a.shaderProgramID = m_shader->getShader();
    m_mesh->setShader(m_shader);

    /*
    m_VAO.bind();
    m_VBO.bind();
    m_VBO.setData(sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    m_VAO.unbind();
    */
}

void Object::setCamera(std::shared_ptr<Camera> camera)
{
    m_camera = camera;
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

GLuint Object::getShaderID() const
{
    // new refactored code
    return m_shader->getShader();
    
    // old
    //return this->shaderProgram;
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

std::shared_ptr<Shader> Object::getShaderProgram()
{
    return this->m_shader;
}

void Object::PrintActiveUniformVariables() const
{
    m_shader->PrintActiveUniformVariables();
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
    return m_mesh->getTextureCount() > 0;
    //return texture != nullptr;
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
    return m_shader->getType();
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


/*
*   Render meshes of the object
*   Default uniforms for PHONG shader
*/
void Object::draw()
{
    // Aktivace shaderu
    m_shader->use();
    
    m_shader->sendUniform("viewMatrix", m_camera->getCamera());
    m_shader->sendUniform("projectionMatrix", m_camera->getProjectionMatrix());

    // Pošleme z CPU do GPU uniformní promìnné aktualizovaných dat
    m_shader->sendUniform("modelMatrix", m_matrix);
    //m_shader.sendUniform("fragmentColor", color);
    // send uniformmeshMaterial
    //m_mesh->setShader(m_shader);
    
    // texture load
    if (hasTexture())
    {
        m_shader->sendUniform("myTextureSampler", (GLint)0);
        // Funkce, která binduje texturu pro použití v shaderu
        // TODO: funkce texture->Bind() musi byt nejspis presunuta do tridi Mesh
        //texture->Bind();
    }

    // viewMatrix a projectionMatrix a viewPos se updatuje ve ShaderProgram classe pøi zmìnì kamery
    
    //GLint stencilValue = 0; // 15
    //glStencilFunc(GL_ALWAYS, stencilValue, 0xFF);
    
    m_mesh->render();
}

// TODO: old, nepouzivam uz pro vykreslovani, takze by se toto melo smazat
// misto toho pouzivam funkci draw()
/*void Object::render()
{
    this->getModel().bindVAO();
    
    this->useShader();  // use shaderProgram
    //glUseProgram(Shader::getShader(getShaderType()));
    
    //Shader::sendUniform(Shader::getShader(getShaderType()), "modelMatrix", getMatrix());
    Shader::sendUniform(getShaderID(), "modelMatrix", getMatrix());

    if (getShaderType() == ShaderType::PHONG //|| (getShaderType() == ShaderType::DIFFUSE))
    {
        if (hasTexture()) {
            loadTexture();
            Shader::sendUniform(getShaderID(), "hasTexture", 1);

            Shader::sendUniform(getShaderID(), "textureUnitID", 0);
            //GLint uniformID = glGetUniformLocation(getShader(), "textureUnitID");
            //glUniform1i(uniformID, 0);
        }
        else {
            Shader::sendUniform(getShaderID(), "hasTexture", 0);
            Shader::sendUniform(getShaderID(), "fragmentColor", getColor());
        }
    }
    
    
    //Shader::sendUniform(getShader(), "fragmentColor", glm::vec3(1.f, 0.0f, 0.0f));

    this->getModel().render();		// glDrawArrays()
}*/
