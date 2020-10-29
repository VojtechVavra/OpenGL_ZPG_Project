#ifndef SHADER_HPP
#define SHADER_HPP

#include <unordered_map>
#include <string>

#include <GL/glew.h> // must be always included first!
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp> 

#include "Observer.hpp"
// http://www.cplusplus.com/forum/beginner/146499/
//#include "Camera.hpp" //The problem is that you can't include an object of a derived class in its parent . Reason being


enum class ShaderType {
    AMBIENT,    // Constant
    DIFFUSE,    // Lambert
    SPECULAR,
    PHONG,      // Phong = Constant + Lambert + Specular
    BLINN
};

static std::unordered_map<ShaderType, std::string> fragShaderPath(
    {
        { ShaderType::AMBIENT,  "./shaders/ambient.frag" },
        { ShaderType::DIFFUSE,  "./shaders/diffuse.frag" },
        { ShaderType::SPECULAR, "./shaders/specular.frag" },
        { ShaderType::PHONG,    "./shaders/phong.frag" },
        { ShaderType::BLINN,    "./shaders/blinn.frag" }
});


class Camera;

class Shader : public Observer {
public:
    Shader();
    Shader(Camera* camera, ShaderType shaderType);

    void sendUniform(const GLchar* name, GLfloat value);
    void sendUniform(const GLchar* name, glm::vec3 dataVec3);
    void sendUniform(const GLchar* name, glm::vec4 dataVec4);
    void sendUniform(const GLchar* name, glm::mat4 dataMat4);

    static void sendUniform(GLuint shaderProgram, const GLchar* name, GLfloat value);
    static void sendUniform(GLuint shaderProgram, const GLchar* name, glm::vec3 dataVec3);
    static void sendUniform(GLuint shaderProgram, const GLchar* name, glm::vec4 dataVec4);
    static void sendUniform(GLuint shaderProgram, const GLchar* name, glm::mat4 dataMat4);

    //void Update();
    //void createMe();

    GLuint createShader(ShaderType fragmentShaderType = ShaderType::PHONG);
    static GLuint createShader(ShaderType fragmentShaderType, int i);

    //GLuint createShader();

    GLuint getShader();
    ShaderType getType();
    void update(std::string change) override;
    void use();
    static void use(GLuint shaderProgram);
private:
    GLuint shaderProgram;
    
    Camera* m_camera;
    ShaderType type;
};

#endif