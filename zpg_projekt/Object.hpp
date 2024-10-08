#ifndef OBJECTSPACE_H
#define OBJECTSPACE_H

#include <vector>
#include <memory>

#include <GL/glew.h>	    // must be always included first!
#include <GLFW/glfw3.h>     // this will include <GL/gl.h>
#include <glm/vec3.hpp>     // glm::vec3
#include <glm/vec4.hpp>     // glm::vec4
#include <glm/mat4x4.hpp>   // glm::mat4

#include "IObserver.hpp"
//#include "Model.hpp"  // dodat
#include "Shader.hpp"
#include "Subject.hpp"

// added new code - after refactoring
#include "VAO.hpp" // added misto Model.hpp ten by se mel smazat a zaniknout
#include "VBO.hpp"
#include "Mesh.hpp"

#include "Texture.hpp"

//#include "Camera.hpp"   // added, maybe throw error
//class Shader;
class camera;

// Object model space
class Object
{
public:
    virtual ~Object() = default;
    Object(glm::vec3 position); // pouzivam jen pro kameru

    // tohle asi se muze smazat - nepouziva se
    //Object(glm::vec3 position, GLuint shaderProgram, ShaderType shaderType); // For lights without model
    //Object(glm::vec3 position, glm::vec3 color, GLuint shaderProgram, ShaderType shaderType);
    //Object(glm::vec3 position, std::shared_ptr<Texture> texture, GLuint shaderProgram, ShaderType shaderType);
    
    Object();

    // nova funkce pro tvorbu objektu s vao a vbo tvorbou
    Object(const std::string& name, const ShaderType shaderType = ShaderType::PHONG);
    
    //~Object();
    void setCamera(std::shared_ptr<Camera> camera);

    glm::vec3 getPosition() const;
    glm::mat4 getMatrix() const;
    glm::vec3 getRotate() const;
    glm::vec3 getScale() const;
    GLuint getShaderID() const;
    
    void setMatrix(glm::mat4 newMatrix);
    void setPosition(glm::vec3 position);
    void Rotate(glm::mat4 mat4x4, float angle, glm::vec3 axis);
    void Rotate(float angle, glm::vec3 axis);
    void Translate(glm::mat4 mat4x4, glm::vec3 shift);
    void Translate(glm::vec3 shift);
    void Scale(glm::mat4 mat4x4, glm::vec3 scale);
    void Scale(glm::vec3 scale);
    //void LookAt(glm::vec3 target, float x, float y);

    // Draw object
    virtual void draw();
    // Render mesh model
    //void render_mesh_new();
    
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
    void PrintActiveUniformVariables() const;

    std::shared_ptr<Shader> getShaderProgram();
    //void update(Camera* camera, camChange cameraChange) override;
    //bool hasTexture = false;
protected:
    glm::vec3 position;
    glm::mat4 m_matrix;   // model matrix

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
    std::shared_ptr<Mesh> m_mesh;
    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<Camera> m_camera;
};

#endif // !OBJECTSPACE_H
