#ifndef OBJECTSPACE_H
#define OBJECTSPACE_H

#include <vector>
#include <memory>

#include <GL/glew.h>	    // must be always included first!
#include <GLFW/glfw3.h>     // this will include <GL/gl.h>
#include <glm/vec3.hpp>     // glm::vec3
#include <glm/vec4.hpp>     // glm::vec4
#include <glm/mat4x4.hpp>   // glm::mat4

#include "Observer.hpp"
#include "Model.hpp"  // dodat
#include "Shader.hpp"
#include "Subject.hpp"

#include "Texture.hpp"

//#include "Camera.hpp"   // added, maybe throw error
//class Shader;
class camera;

// Object model space
class Object // : public virtual Observer
{
public:
    Object(glm::vec3 position); // pouzivam jen pro kameru
    Object(glm::vec3 position, GLuint shaderProgram, ShaderType shaderType); // For lights without model
    Object(glm::vec3 position, Model model, glm::vec3 color, GLuint shaderProgram, ShaderType shaderType);
    Object(glm::vec3 position, Model model, std::shared_ptr<Texture> texture, GLuint shaderProgram, ShaderType shaderType);
    
    //Object(glm::vec3 position, Model model, glm::vec3 color, GLuint shaderID, ShaderType shaderType, std::shared_ptr<Camera> camera);
    //Object(glm::vec3 position, Model model, GLuint shaderID, ShaderType shaderType);
    Object();
    //~Object();
    glm::vec3 getPosition() const;
    glm::mat4 getMatrix() const;
    glm::vec3 getRotate() const;
    glm::vec3 getScale() const;
    Model getModel() const;
    GLuint getShader() const;

    void useShader();
    void setMatrix(glm::mat4 newMatrix);
    void setPosition(glm::vec3 position);
    void Rotate(glm::mat4 mat4x4, float angle, glm::vec3 axis);
    void Rotate(float angle, glm::vec3 axis);
    void Translate(glm::mat4 mat4x4, glm::vec3 shift);
    void Translate(glm::vec3 shift);
    void Scale(glm::mat4 mat4x4, glm::vec3 scale);
    void Scale(glm::vec3 scale);
    //void LookAt(glm::vec3 target, float x, float y);

    void render();
    void renderSkybox(std::shared_ptr<Camera> camera);
    void renderSkybox2(std::shared_ptr<Camera> camera);

    bool hasTexture();
    void loadTexture();

    glm::vec3 getColor();
    void setNewColor(glm::vec3 newColor);
    void setLastColor();
    ShaderType getShaderType();
    GLuint getID();
    //void update(std::string change) override;
    //void update(Camera* camera, camChange cameraChange) override;
    void setPositionWithoutTranslate(glm::vec3 newPosition);

    //bool hasTexture = false;
protected:
    glm::vec3 position;
    glm::mat4 m_matrix;   // model matrix
    Model model;
    GLuint shaderProgram;   // originally shaderID
    ShaderType shaderType;
    //std::shared_ptr<Camera> camera;
public:
    std::shared_ptr<Texture> texture;

    glm::vec3 rotation;
    glm::vec3 scale;
protected:
    glm::vec3 color;
    glm::vec3 lastColor;
    static GLuint objectCount;
    GLuint objID;
private:
    //glm::vec3 position;
    
};

#endif // !OBJECTSPACE_H
