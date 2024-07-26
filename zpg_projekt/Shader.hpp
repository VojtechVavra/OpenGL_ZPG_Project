#ifndef SHADER_HPP
#define SHADER_HPP

#include <unordered_map>
#include <string>

#include <GL/glew.h> // must be always included first!
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp> 

#include "Observer.hpp"

//#include "Subject.hpp"
// http://www.cplusplus.com/forum/beginner/146499/
//#include "Camera.hpp" //The problem is that you can't include an object of a derived class in its parent . Reason being

class ShaderProgram;

enum class ShaderType {
    AMBIENT,    // Constant
    DIFFUSE,    // Lambert
    SPECULAR,
    PHONG,      // Phong = Constant + Lambert + Specular
    BLINN,
    SKYBOX,
    DIFFUSE_MODEL,
    SPECULAR_MODEL,
    GRAB_MODEL,
    FLAME
};

static std::unordered_map<ShaderType, std::string> fragShaderPath(
    {
        { ShaderType::AMBIENT,          "./shaders/ambient.frag" },
        { ShaderType::DIFFUSE,          "./shaders/diffuse.frag" },
        { ShaderType::SPECULAR,         "./shaders/specular.frag" },
        { ShaderType::PHONG,            "./shaders/phong.frag" },
        { ShaderType::BLINN,            "./shaders/blinn.frag" },
        { ShaderType::SKYBOX,           "./shaders/skybox.frag" },
        { ShaderType::DIFFUSE_MODEL,    "./shaders/TextureFragmentShader.frag" },
        { ShaderType::SPECULAR_MODEL,   "./shaders/------" },    // dodelat specularni shader pro 3d modely/objekty
        { ShaderType::GRAB_MODEL,       "./shaders/grabObject.frag" },    // dodelat specularni shader pro 3d modely/objekty
        // DIFFUSE_MODEL a SPECULAR_MODEL are shaders for imported models
        { ShaderType::FLAME,            "./shaders/flame.frag" }
});

static std::unordered_map<ShaderType, GLuint> shaderPrograms(
    {
        { ShaderType::AMBIENT,  0 },
        { ShaderType::DIFFUSE,  0 },
        { ShaderType::SPECULAR, 0 },
        { ShaderType::PHONG,    0 },
        { ShaderType::BLINN,    0 },
        { ShaderType::SKYBOX,   0 },
        { ShaderType::DIFFUSE_MODEL,    0 },
        { ShaderType::SPECULAR_MODEL,   0 },
        { ShaderType::GRAB_MODEL,   0 },
        { ShaderType::FLAME,   0 }
    });

class Camera;

class Shader { //: public virtual Observer {
public:
    void sendUniform(const GLchar* name, GLfloat value);
    void sendUniform(const GLchar* name, glm::vec3 dataVec3);
    void sendUniform(const GLchar* name, glm::vec4 dataVec4);
    void sendUniform(const GLchar* name, glm::mat4 dataMat4);

    static void sendUniform(GLuint shaderProgram, const GLchar* name, GLint value);
    static void sendUniform(GLuint shaderProgram, const GLchar* name, GLfloat value);
    static void sendUniform(GLuint shaderProgram, const GLchar* name, glm::vec3 dataVec3);
    static void sendUniform(GLuint shaderProgram, const GLchar* name, glm::vec4 dataVec4);
    static void sendUniform(GLuint shaderProgram, const GLchar* name, glm::mat4 dataMat4);

    //GLuint createShader(ShaderType fragmentShaderType = ShaderType::PHONG);
    static GLuint createShader(ShaderType fragmentShaderType);
    static GLuint getShader(ShaderType fragmentShaderType);
    static std::vector<ShaderProgram> getShaderPrograms();

    GLuint getShader();
    ShaderType getType();

    // aktivuje shader (nutne pred vykreslovanim objektu a pred zaslanim uniformnich promennych do shaderu)
    void use() const;
    static void use(GLuint shaderProgram);

private:  
    GLuint shaderProgram;
    ShaderType type;
};

#endif