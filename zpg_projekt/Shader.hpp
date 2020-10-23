#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h> // must be always included first!
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp> 
// http://www.cplusplus.com/forum/beginner/146499/
//#include "Camera.hpp" //The problem is that you can't include an object of a derived class in its parent . Reason being

class Camera;

class Shader {
public:
    Shader();
    Shader(Camera* camera);
    Shader(int i);
    //GLuint CreateAndCompile();
    void sendUniform(const GLchar* name, GLfloat value);
    void sendUniform(const GLchar* name, glm::vec3 dataVec3);
    void sendUniform(const GLchar* name, glm::vec4 dataVec4);
    void sendUniform(const GLchar* name, glm::mat4 dataMat4);
    //void Update();
    void createMe();
    GLuint createShader();
    GLuint getShader();
    void use();
private:
    GLuint shaderProgram;
    //void VertShaderCompileError(GLuint shader);
    //void FragShaderCompileError(GLuint shader);
    Camera* m_camera;

    /*const char* vertex_shader =
        "#version 420\n"    // 330
        "uniform mat4 modelMatrix;"                   // = mat4(1.0f);"
        "uniform mat4 viewMatrix = mat4(1.0f);"       // = mat4(1.0f);"
        "uniform mat4 projectionMatrix = mat4(1.0f);" // = mat4(1.0f);"   // perspective or orthographic effect
        "uniform vec3 newLightPosition;"

        "layout(location=0) in vec3 vp;"
        "layout(location = 1) in vec3 vertNormal;"
        //"layout(location=1) in vec3 vertexColor;" //  // Notice that the 1 here equals the 1 in glVertexAttribPointer

        "out vec4 ex_worldPosition;"
        "out vec3 ex_worldNormal;"
        "out vec3 fragmentColor;"
        "out vec3 lightPosition;"

        "void main () {"
        //  "gl_Position = mProj * mView * mModel * vec4(position, 1.0);"
        "   gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vp, 1.0);"  // gl_position became normalized device coordinates (ranging from -1 to +1)
        "   ex_worldPosition = modelMatrix * vec4(vp, 1.0);"
        //"   ex_worldNormal = (M)*vec4(normal, 1.0);"
        "   ex_worldNormal = vec3(1.0f);"
        //"   fragmentColor = vertexColor;"
        "   fragmentColor = vec3(1.0f, 0.5f, 0.0f);"
        "   lightPosition = newLightPosition;"
        "}";
        */

    // in vertex shader 
    /*const char* vertex_shader_v2 = "#version 330\n"
    "uniform mat4 trans;"
    "mat4 aMat4 = mat4(1.0, 0.0, 0.0, 0.0,"
                  "0.0, 1.0, 5.0, 0.0,"
                  "0.0, 3.0, 1.0, 0.0,"
                  "0.0, 0.0, 0.0, 1.0);"
    "layout(location=0) in vec3 vp;"
    "void main () {"
    " gl_Position = trans * vec4 (vp, 1.0);"
    "}";*/

    /*const char* vertex_shader_v1 = "#version 330\n"
    "uniform mat4 trans;"
    "layout(location=0) in vec3 vp;"
    "void main () {"
    " gl_Position = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0) * vec4 (vp, 1.0);"
    "}";*/



    /*const char* fragment_shader =
        "#version 400\n"
        "in vec3 fragmentColor;"
        "in vec4 ex_worldPosition;" // fragPos
        "in vec3 ex_worldNormal;"   // normal
        "in vec3 lightPosition;"
        //"uniform vec3 lightPosition;"   //the shader has light position
        //"uniform vec3 viewPos;"
        "uniform vec3 lightColor = vec3(1.0, 0.0, 0.5);"
        //"out vec4 frag_colour; "
        "out vec4 out_Color;" // original name: frag_colour
        "void main () {"
        "//ambient"
        "   float ambientStrength = 1.0f;"
        "   vec3 ambient = ambientStrength * lightColor;"
        "//diffuse"
        "   vec3 norm = normalize(ex_worldNormal);"
        "   vec3 lightDir = normalize(lightPosition - ex_worldPosition);"
        "   float diff = max(dot(norm, lightDir), 0.0);"
        "   vec3 diffuse = diff * lightColor;"
        "//result"
        // "   vec3 result = (ambient + diffuse) * objectColor;"
        "   vec3 result = (ambient + diffuse) * fragmentColor;"
        "   out_Color = vec4(result, 1.0);"
        "}";
        */
};

    // nefunguje
    //"vec3 lightVector = normalize(lightPosition);" //spravne upravit"
    //"float dot_product = max(dot(lightVector, normalize(ex_worldNormal)), 0.0f);"
    //"vec4 diffuse = dot_product * vec4(0.385f, 0.647f, 0.812f, 1.0f);"
    //"vec4 ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);"
    //"out_Color = ambient + diffuse;"
    //"out_Color = diffuse;"


/*
    "//ambient"
    "   float ambientStrength = 1.0f;"
    "   vec3 ambient = ambientStrength * lightColor;"
    "// diffuse"
    "   vec3 norm = normalize(ex_worldNormal);"
    "   vec3 lightDir = normalize(lightPos - ex_worldPosition);"
    "   float diff = max(dot(norm, lightDir), 0.0);"
    "   vec3 diffuse = diff * lightColor;"
    "// specular"
    "   float specularStrength = 0.5f;"
    "   vec3 viewDir = normalize(viewPos - ex_worldPosition);"
    "   vec3 reflectDir = reflect(-lightDir, norm);"
    "   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);"
    "   vec3 specular = specularStrength * spec * lightColor;"

    "   vec3 result = (ambient + diffuse + specular) * objectColor;"
    "   color = vec4(result, 1.0f);"
*/

#endif