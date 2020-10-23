#ifndef OBJECTSPACE_H
#define OBJECTSPACE_H

#include <vector>
#include <GL/glew.h>	// must be always included first!
#include <GLFW/glfw3.h> // this will include <GL/gl.h>
#include <glm/vec3.hpp>     // glm::vec3
#include <glm/vec4.hpp>     // glm::vec4
#include <glm/mat4x4.hpp>   // glm::mat4
#include "Model.hpp"  // dodat
#include "Shader.hpp"
//class Shader;

// Object model space
class Object
{
public:
    Object(glm::vec3 position);
    Object(glm::vec3 position, Model model, GLuint shaderID);
    Object();
    ~Object();
    glm::vec3 getPosition() const;
    glm::mat4 getMatrix() const;
    Model getModel() const;
    GLuint getShader() const;
    void useShader();
    void setPosition(glm::vec3 position);
    void Rotate(glm::mat4 mat4x4, float angle, glm::vec3 axis);
    void Rotate(float angle, glm::vec3 axis);
    void Translate(glm::mat4 mat4x4, glm::vec3 shift);
    void Translate(glm::vec3 shift);
    void Scale(glm::mat4 mat4x4, glm::vec3 scale);
    void Scale(glm::vec3 scale);
protected:
    glm::vec3 position;
    glm::mat4 m_matrix;   // model matrix
    Model model;
    GLuint shaderID;
    //GLuint id;      // shader program object ID in scene -> in shader object
private:
    
    //Model* model;
    //Shader* shader;
};

#endif



//mat4x4 – Input matrix multiplied by this rotation matrix.
//angle – Rotation angle expressed in radians.
//axis – Rotation axis, recommended to be normalized.
//Note that to convert from degrees to radians, use glm::radians(degrees)